{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE TupleSections #-}
{-# LANGUAGE LambdaCase #-}

module CodeGen where

import Data.Maybe
import Types
import Utils
import CConstructs
import qualified Data.Map as M
import Control.Monad.State
import Control.Lens
import Control.Applicative

debug = undefined


runConDecl_ :: ConDecl -> ([C_TopLevel], Env)
runConDecl_ c = runState (evalConDecl c) initialEnv

runProgram_ :: Program -> ([C_TopLevel], Env)
runProgram_ c = runState (evalProgram c) initialEnv

runConDecl = fst . runConDecl_
runProgram = fst . runProgram_

toStatements :: [C_TopLevel] -> [Statement]
toStatements top = vars ++ structs ++ funs
    where
      funs = top ^. traverse._C_Fun._2
      structs = top ^. traverse._C_Struct._2
      vars = top ^. traverse._C_Var._2

renderProgram :: [Statement] -> String
renderProgram = unlines

toProgram = renderProgram . toStatements . runProgram
pprog = putStrLn . toProgram
               
initialEnv = Env M.empty Nothing 0 M.empty []


{-
Compilation process:
	First every data declaration gives rise to a new c file and a header file.
	Every top level function definition is scanned, its presence loaded into the env and its info table initializations are generated.
	The presence of the main function is verified.
	Then the main initialization that calls into the initilzations of the data declarations and the functions and ends with a call to fast entry point of the function is generated.
	Then the fast and slow entry points of the function bodies are generated.
	includes for my rts c library as well as the data declarations are generated.
	Dependency analysis of the function bodies is done and they are rendered.
	The gcc command with the appropriate includes is generated and invoked and if all goes well compilation succeeds.


--------------------------------------------------------------------------------

Need a mapping from the top-level function names, to the function info table representation
Need to store the map from var name to integer key
Need to store the current function name for generating case/thunk continuation names
Need to provide a way to get a fresh variable name that is unused
Need a mapping from constructor names to the constructor info table

Need to also store a list of functions to be genned and the deps they have

--------------------------------------------------------------------------------

    generatePapSize = undefined -- generate the pap size from the atoms
    assignPapAtoms pap fun atoms = undefined -- gen from env
    toPrimOpArgs = undefined -- gen from env
    knownAndSaturated f as = undefined -- gen from env
    pushFunArgs = undefined -- gen from env
    [AltForce x e] -> undefined -- TODO Need to handle this case.
    var_key = undefined -- get from the environment
    func_prefix = undefined -- get from the environment
    conInnerName = undefined -- gen from env
    expectedConNum = undefined -- get from the environment
    case_con_name = undefined -- gen from env and record that a case needs to be built
    tmp = undefined -- gen from env
    updateKey = undefined -- get from the environment (need map from var -> updKey)
    where thunk_cont = undefined -- get from the environment, generate a new thunk cont name
    c_info_table = undefined -- get from env
    fields = undefined -- get from env
    pap_info_name = undefined -- gen from the environment
    funInfoTable = undefined -- gen from the environment

--------------------------------------------------------------------------------


Need to generate the contents for the header files as well.

Constructor definitions must be pre-processed seperately
Notes:
Env:
Binding key of free variables

Mapping from var to it's updatekey
queue of (thunk expressions, thunk names) to generate functions for

Mapping from the constructor name to its info table in Haskell
  info table must have all the fields
Mapping from the constructor name to its accessor names
  This should also go in a seperate file and directory and create a c and header file
  Also we only deal with top-level definitions of function currently
For a case, generate a note indicating that you have to generate the actual continuation
(the continuation needs to come before the place where the case is used. maybe just prepended -
will need to save the state somehow - maybe add a specific comment in the header.
The deps should be acyclic, Might be a good idea to compile functions as an individual cunit with
some metadata like the dependencies and then have the final thing that renders them have fun.

We need to generate the header files as well:
Everytime you generate a function you'll need to log if it goes in the header file.
You also need to log which properties go in the header file.
You also need to log which struct definitions go in the header file.

Need to generate a makefile as well.

-}

-- data Env = Env { _funMap :: FunMap, _curFun ::  Maybe CurFun, _freshNameSource :: FreshNameSource, _conMap :: ConMap, _deferred :: [MonStack C_TopLevel] }

    



-- Probably a source of bugs - we discard the type information here (just bad design really)
generateFunction :: [C_TopLevel] -> String -> MonStack [Statement] -> [(Type, Arg)] -> MonStack C_TopLevel
generateFunction deps name fun_stmts args = do
  funMap.ix name .= funDescr
  curFun .= Just name
  stmts <- fun_stmts
  curFun .= Nothing
  return (C_Fun name stmts deps)
      where
        funDescr = FInf name (length args) (map snd args) layoutEnts
        (layoutEnts, offsets) = unzip $ zipWith toLayoutEntry ("0":offsets) args
            where
              toLayoutEntry off (_, (_, t))
                  = case t of
                      Boxed -> let size = "sizeof(ref)" in
                               let pos = s "$$ + $$" [off, size] in
                               (LayoutEntry size True pos, pos)
                      Unboxed -> let size = "sizeof(int)" in
                                 let pos = s "$$ + $$" [off, size] in
                                 (LayoutEntry size False pos, pos)

freshInt :: MonStack Int
freshInt = do
  x <- use freshNameSource
  freshNameSource += 1
  return x

freshName :: MonStack String
freshName = do
  x <- freshInt
  return (to_temp_var x)

freshIntStream :: MonStack [Int]
freshIntStream = let stream = (:) <$> freshInt <*> stream in stream
  

generatePapSize :: String -> Int -> MonStack String
generatePapSize fun argLen = do
  -- you want the first n arguments.
  args <- use (funMap.ix fun.finfArgs)
  return . c_sum . map (toSize . snd)  . take argLen $ args

{-
     *(ref*)(pap + 1) = one_ref;
-}
assignPapAtoms pap fun atoms = do
  layout <- use (funMap.ix fun.finfLayout)
  return $ zipWith toAssignment layout atoms
      where
        toAssignment (LayoutEntry _ isPtr offs) a
            = if not isPtr
              then deref (castPtr "int" location) ..= atomToVal a
              else deref (castPtr "ref" location) ..= atomToVal a
            where
              atomToVal (L i) = show i
              atomToVal (V v) = v
              location = s "$$ + $$" [castPtr "char" pap, offs]

knownAndSaturated :: String -> Int -> MonStack Bool
knownAndSaturated f as = do
  args <- use (funMap.ix f.finfArgs)
  return $ (length args) == as

pushFunArgs :: String -> [Atom] -> MonStack [Statement]
pushFunArgs fun args = do
  argTypes <- use (funMap.ix fun.finfArgs)
  return $ zipWith toPushInstr args argTypes
      where
        toPushInstr (V x) (_, Boxed) =  s "push_ptr($$);" [x]
        toPushInstr (V x) (_, Unboxed) =  s "push_int($$);" [x]
        toPushInstr (L i) (_, Unboxed) =  s "push_int($$);" [show i]
                               
toCType a@(x, Boxed) = ("ref", a)
toCType a@(x, Unboxed) = ("int", a)


{-
// TODO: intialize the layout for every function info table, (plus_int, map)

ref map_fast(ref function, ref list)
{
    struct hash_map *bindings;
    init_bindings(&bindings);

    put_binding(bindings, 0, function);
    put_binding(bindings, 1, list);
    return map_case_cont(bindings);
}

init_function_map()
{
  struct arg_entry *plus_entries = (struct arg_entry*)new(sizeof(struct arg_entry)*2);
  plus_entries[0] = (arg_entry) { .size = sizeof(ref), .pointer = true, .offset = 0 };
  plus_entries[1] = (arg_entry) { .size = sizeof(ref), .pointer = true, .offset = sizeof(ref) };

  plus_info_table.type = 0;
  plus_info_table.extra.function = (struct fun) { .slow_entry_point = plus_int_slow, .arity = 2 };
  plus_info_table.layout = (struct layout) { .num = 2, .entries = plus_entries };

}
Also we only deal with top-level definitions of function currently
-}
init_arg_entry :: (Int, (String, (String, ValueType))) -> (String, String)
init_arg_entry (ix, (offset, (argName, argType))) = case argType of
        Boxed -> (arrayIx ..= structValue (fields "true" "sizeof(ref)"), s "$$ + sizeof(ref)" [show offset])
        Unboxed -> (arrayIx ..= structValue (fields "false" "sizeof(int)"), s "$$ + sizeof(int)" [show offset])
  where
    arrayIx = arrayIndex "layout_entries" ix
    structValue = bracketInit "arg_entry"
    fields isPointer argSize = [("size", argSize), ("pointer", isPointer), (".offset", show offset)]

evalProgram :: Program -> MonStack [C_TopLevel]
evalProgram = fmap concat . mapM generateTopLevelDefn
    where
      generateTopLevelDefn (name, FUNC (Fun args e))
          = do
        let slowEntryPointDecl = C_Fun slow_entry_name [] 
        -- TODO might need to generate a var representing the var name
        infoTable <- generateFunction [] info_table_name info_table_initializer_stmts []
        fastEntry <- generateFunction [] fast_entry_name fast_entry_point (map toCType args)
        slowEntry <- generateFunction [] slow_entry_name slow_entry_point []
        return [infoTable, fastEntry, slowEntry]
          where
            (initializeLayoutEntries, offsets) = unzip $ map init_arg_entry $ zip [0..] $ zip ("0":offsets) args
            info_table_function = Fun [] 
            (info_table_name, info_table_initializer_stmts) = (init_func_name, funcFormatter "void" init_func_name [] <$> body)
                where
                  init_func_name = s "init_function_$$" [name]
                  body = return $ initLayout name args ++ [funInfoTableName name ..= info_table_struct]
                  initLayout name args = allocateLayoutObject:initializeLayoutEntries
                    where
                      allocateLayoutObject = declInit "arg_entry*" "layout_entries" $ castPtr "arg_entry" $ funCall "new" $ [s "sizeof(arg_entry)*$$" [show $ length args]]
                  layout = bracketInit "struct layout" [("num", "2"), ("entries", "layout_entries")]
                  info_table_struct = bracketInit "info_table" $
                            [("type", "0"),
                             ("extra", functionStruct),
                             ("layout", layout)
                            ]
            functionStruct = bracketInit "function" [("slow_entry_point", slow_call_name name), ("arity", show $ length args)]

            slowEntryPointDescr = FInf slow_entry_name 0 [] []
            (slow_entry_name, slow_entry_point) = (slow_call_name name, generateSlowCall name . map (\x -> (V . fst $ x, snd x)) $ args)
--            fastEntryPointDecl = C_Fun fast_entry_name fast_entry_point []
            fastEntryPointDescr = FInf fast_entry_name (length args) args
            fast_entry_name = fast_call_name name
            fast_entry_point = funcFormatter "ref" (fast_call_name name) fastArgs <$> body
                where
                  fastArgs = map f args
                  newBindings =  zip (fst . unzip $ args) [0..]
                  body = ((initBindings ++ map (uncurry putBinding) newBindings) ++) <$> eval (M.fromList newBindings) e
                  f (v, Boxed) = ("ref", v)
                  f (v, Unboxed) = ("int", v)
{-
ref map_slow(ref null)
{
  if(arg_satisfaction_check(sizeof(ref)*2))
  {
    ref arg1;
    ref arg2;
    pop_ptr(&arg1);
    pop_ptr(&arg2);
    return map_fast(arg1, arg2);
  }
  else
  {
     // at least 1 arg must be present
     ref arg1;
     pop_ptr(&arg1);

     NEW_REF(pap_ref, void**, sizeof(void*) + sizeof(ref), pap_)

     struct info_table* pap_info = (struct info_table*)new(sizeof(struct info_table));
     pap_info->type = 4;
     pap_info->extra.pap_info = (struct pap){ .info_ptr = &map_info_table, .size = 1 };
     pap_[0] = pap_info;
     *(ref*)(pap_ + 1) = arg1;
     return pap_ref;
  }
}
-}
generateSlowCall name args = do
  elseSt <- elseSt_
  let body = ifSt argSatisfactionCondition (generateFastCallFromArgsOnStack name args) [elseSt]
  return $ funcFormatter "ref" (slow_call_name name) [("ref", "null")] body
   where
     argSatisfactionCondition = funCall "arg_satisfaction_check" [argSize]
     argSize = c_sum (map (toSize . snd) args)
     elseSt_ = do
       initArgs <- initArgs_
       return $ map toDecl args ++ map toPopInstr args ++ [newRefMacro "pap_ref" "void**" argSize "pap_"] ++ initStructTable ++ initArgs ++ [returnSt "pap_ref"]
                  where
                    initStructTable = [declInit "struct info_table*" "pap_info" (castPtr "struct info_table*" (funCall "new" ["sizeof(info_table)"])),
                                       ptrAccess "pap_info" "type" ..= "4",
                                       ptrAccess "pap_info" "extra.pap_info" ..= infoTableStruct,
                                       arrayIndex "pap_" 0 ..= "pap_info"]
                    infoTableStruct = bracketInit "struct pap" [("info_ptr", "&pap_info"), ("size", "1")]
                    initArgs_ = evalObject M.empty "pap_" (PAP (Pap name args_))
                    args_ = fst . unzip $ args
                    

     generateFastCallFromArgsOnStack name args
         = map toDecl args
           ++ map toPopInstr args
           ++ [returnSt (funCall (fast_call_name name) [commaSep . map (unwrap . fst) $ args])]
     unwrap (V x) = x
     toDecl (V x, Boxed) = decl "ref" x
     toDecl (V x, Unboxed) = decl "int" x
     toPopInstr (V x, Boxed) = funCall "pop_ptr" . (:[]) . reference $ x
     toPopInstr (V x, Unboxed) = funCall "pop_int" . (:[]) . reference $ x


{-
data List a = Cons { value :: a, next :: (List a) } | Nil

//
typedef struct Cons {
    struct info_table *info_ptr;
    ref value;
    ref next;
} Cons;


void init_list()
{
    cons_info_table = (struct info_table){ .type = 1, .extra = { .constructor = { .arity = 2, .con_num = 0 } } };

    nil_info_table = (struct info_table){ .type = 1, .extra = { .constructor = { .arity = 0, .con_num = 1 } } };

    new_ref(sizeof(Nil), &nil_value);
    ((Nil*)get_ref(nil_value))->info_ptr = &nil_info_table;
}

-}
-- Needs to generate the struct and the initialization of its info table
evalConDecl :: ConDecl -> MonStack [C_TopLevel]
evalConDecl (ConDecl typeName cons) = do
  conMap %= flip (foldl conMapUpd) cons
  funcDecl <- generateFunction structDecls name (funcFormatter "void" name args <$> body) args
  return (funcDecl:structDecls)
    where
      info_table_name c = s "$$_info_table" [c]
      conMapUpd mp c = M.insert (conName c) c mp
      structDecls = map toStructDecl cons
      toStructDecl (ConDefn conName _ fields) = C_Struct conName (decl "struct info_table" (info_table_name conName):typedef conName fields) []
      structInit = map (typedef typeName . conFields) $ cons
      name = s "init_constructors_$$" [typeName]
      args = []
      body = return $ map generateConDefn cons
      generateConDefn (ConDefn conName tag l)
          = info_table_name conName  ..= bracketInit "info_table" bs
            where
              bs = [ ("type", "1"), ("extra", extra) ]
              extra = bracketInit "union info_table_u"
                        [("constructor",
                          bracketInit "con"
                             [("arity", show $ length l),
                              ("tag", show tag)]
                         )]


{-

// no default patterns
case l of
	Cons x xs -> x

ref cont(hash_map *bindings)
{
	GETBINDING(l_ref, void**, l, <l key>, bindings)
	info_table *l_info = *(info_table **)l;
	if(l_info->type == 1)
	{
		assert(l_info->extra.constructor.con_num == <con_num>);
		Cons *c = (Cons*)l;
		return c-><field>;
	}
	else
	{
		// if the matches were for a constructor then this must be a thunk
		assert(l_info->type == 5);
		return thunk_continuation(l_ref, cont, bindings, update_key, l_ref);
	}
}
-}

-- TODO: a case doesn't actually compile to this, this is just the case continuation
-- TODO: A case actually compiles to a return case_name(bindings);
generateCaseCont :: String -> Bindings -> Expression -> MonStack C_TopLevel
generateCaseCont name bindings (Case (V var_name) es) = do
    let statements = funcFormatter returnType name args <$> thunkBody name
    generateFunction [] name statements (args & traverse._2 %~ (,Boxed))
  where
    thunkBody name = case es of
        -- For this case we push a case frame and then a 'fake' update frame that restores su and returns the arg that the update frame was called with 
        [AltForce x e] -> undefined -- TODO Need to handle this case.
        alts -> pure ([
            bindingMacro var_ref "void**" var_name var_key "bindings",
            declInit "info_table*" info_table (deref (castPtr "info_table*" var_name))
          ] ++) <*> (ifSt conCase <$> (fmap concat . mapM caseIf $ alts) <*> pure [elseSt name])

    conCase = s "$$->type == 1" [info_table]    
    var_key = show . al $ M.lookup var_name bindings
    elseSt name = [
       assert (s "$$ == $$" [ptrAccess info_table "type", "5"]),
       returnSt (funCall "thunk_continuation" [var_ref, name, "bindings", var_ref])
     ]
    caseIf (AltCase conName freeVars exp) = do
            [conDefn] <- uses (conMap.ix conName) (:[])
            let expectedConNum = conTag conDefn
                cond = assert (s "$$ == $$" [actualConNum, show expectedConNum])
            ifBody <- do
               conInnerName <- freshName
               intStream <- freshIntStream
               let init_con_struct = declInit (s "$$*" [conName]) conInnerName conCasted
                   freeVarBindings = zip (conFields conDefn) intStream
                   bindings' = foldl updBindings bindings freeVarBindings
                   bindCaseStmts = freeVarBindings >>= genBindConVarStmts 
                   updBindings mp ((name, _), i) = M.insert name i mp
                   genBindConVarStmts ((field_name, value_type), num)
                       = case value_type of
                           Unboxed -> [declInit "int*" tmp_var (reference con_field_val), putBinding tmp_var num]
                           Boxed -> [declInit "ref" tmp_var con_field_val, putBinding tmp_var num]
                            -- need to actually assign the new variables from conInnerName, then need to generate statements to bind the new names to the new ints
                       where
                         tmp_var = to_temp_var num
                         con_field_val = ptrAccess conInnerName field_name
                         
               case_alt_stmts <- eval bindings' exp
               
               return $ init_con_struct: bindCaseStmts ++ case_alt_stmts

            return $ ifSt cond ifBody []
        where
          conCasted = castPtr conName var_name
          actualConNum = structAccess (structAccess (ptrAccess info_table "extra") "constructor") "con_num"
    info_table = s "$$_info" [var_name]
    var_ref = s "$$_ref" [var_name]
    returnType = "ref"
    args = [("hash_map*", "bindings")]


{-
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref one_ref;
     get_binding(bindings, 0, &one_ref);
     push_ptr(one_ref);

     return plus_int_slow(one_ref);

-}    
generateThunkCont bindings funName (THUNK e)
    = do
  let fun_stmts = funcFormatter "ref" funName [("ref", "thunk_ref")] <$> body
  generateFunction [] funName fun_stmts [("ref", ("thunk_ref", Boxed))]
    where
      body = (declInit "hash_map*" "bindings" "THUNK_GET_BINDINGS(thunk_ref)":) <$> eval bindings e

    
eval :: Bindings -> Expression -> MonStack [String]
eval bindings c@(Case _ _) = do
  func_prefix <- freshName
  let name = s "$$_$$" [func_prefix, "cont"]
  deferred %= (generateCaseCont name bindings c:)
  return [returnSt (funCall name ["bindings"])]


-- (void**)get_binding()
-- TODO Need to handle the actual primops cases
eval bindings (Primop "+#" [L (I x), L (I y)]) = eval bindings (Atom . L . I $ x + y)
eval bindings (FuncCall fun args) = do
  cond <- knownAndSaturated fun (length args)
  if cond
  then return [funCall (fast_call_name fun) (map extractArgsToFunArgs args)]
  else do
    tmp <- freshName
    pushStmts <- pushFunArgs fun args
    return $ pushStmts ++ [decl "ref" tmp, funCall (slow_call_name fun) [tmp]]


{-
   let x' = THUNK (f x) in e

   ref thunk1_ref = create_thunk(bindings, map_thunk1);
   put_binding(bindings, 4, thunk1_ref);

-}
eval bindings (Let var obj e) = do
                     updateKey <- freshInt
                     let bs = [putBinding thunk_ref_name  (show updateKey)]
                     thunkObj <- evalObject bindings thunk_name obj
                     rest <- eval (M.insert thunk_ref_name updateKey bindings) e
                     return $ thunkObj ++ bs ++ rest
    where
      thunk_name = var
      thunk_ref_name = s "$$_ref" [thunk_name]


eval bindings (Atom (L (I x))) = return [show x]
eval bindings (Atom (V x)) = return [x]


evalObject :: M.Map String Int -> String -> Object -> MonStack [String]
evalObject bindings obj_name t@(THUNK e) = do
  thunk_cont <- freshName
  deferred %= (generateThunkCont bindings thunk_cont t:)
  return [declInit "ref" obj_name $ funCall "createThunk" ["bindings", thunk_cont]]
{-
   Cons x' f'
//
   NEW_REF(cons_ref, Cons*, sizeof(Cons), cons)
   cons->info_ptr = &cons_info_table;
   cons->value = thunk1_ref;
   cons->next = thunk2_ref;
-}
evalObject bindings obj_name (CON (Con c atoms)) = do
  [constrDefn] <- uses (conMap.ix c) (:[])
  let fields = fst . unzip . conFields $ constrDefn
      assignFields = zipWith assignField fields atoms
  return $ [newRefMacro ref_name (s "$$*" [c]) (s "sizeof($$)"[c]) val_name] ++ assignInfoPtr:assignFields
    where
      val_name = obj_name
      c_info_table = s "$$_info_table" [c]
      ref_name = s "$$_ref" [val_name]
      assignField f (V x) = (ptrAccess val_name f) ..= x
      assignField f (L (I x)) = ptrAccess val_name f ..= show x
      assignInfoPtr = ptrAccess val_name "info_ptr"  ..= (s "&$$" [c_info_table])
{-
     let z = map f
//
     NEW_REF(pap_ref, void**, sizeof(void*) + sizeof(ref), pap)

     NEW(struct info_table, pap_info)
     pap_info->type = 4;
     struct pap pap_info_extra = { .info_ptr = &plus_info_table, .size = 1 };
     pap_info->extra.pap_info = pap_info_extra;

     pap[0] = (void*)pap_info;
     *(ref*)(pap + 1) = one_ref;


-}
evalObject bindings obj_name (PAP (Pap fun atoms)) = do
  papSize <- generatePapSize fun (length atoms)
  [funInfoTable] <- uses (funMap.ix fun.finfName) (:[])
  let pap_info_name = obj_name
      value_name = obj_name
      ref_name = s "$$_ref" [value_name]
      newPap = newRefMacro ref_name "void**" papSize value_name
      newPapInfo = newMacro "info_table" pap_info_name
      assignType = ptrAccess pap_info_name "type" ..= show 4
      pap_info_extra_name = s "$$_extra" [pap_info_name]
      initExtraInfo = declInit "struct pap" pap_info_extra_name (s " { .info_ptr = &$$, .size = $$" [funInfoTable, show (length atoms)])
      assignExtraInfo = structAccess (ptrAccess pap_info_name "extra") "pap_info" ..= pap_info_extra_name
      constructPapInfo = [newPapInfo, assignType, initExtraInfo, assignExtraInfo]
      assignPapInfo = arrayIndex value_name 0 ..= castPtr "void" pap_info_name
  assignPapValues <- (assignPapInfo:) <$> assignPapAtoms value_name fun atoms
  return $ [newPap] ++ constructPapInfo ++ assignPapValues
