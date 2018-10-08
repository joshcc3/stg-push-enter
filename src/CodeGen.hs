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
import Data.List

debug = undefined

-- TODO: Is there a nice way of implementing a lazy stateful infinite stream that doesn't highjack the state (freshIntStream could be infinite)

{-
  Buglist:
   - the thunk cont didn't push any args onto the stack
   - FuncCall is not hitting the saturated case although it show


   - The main function is not generating a call to the inits, 

   - I dont seem to handle higher order functions - just add logic to generateCaseCont to always handle all types of heap objects (except fun of course - although this may be poss)
   - returning primops needs to be done.
   - If an expression is evaluated and is the last statement it should return - need to prove this

-}


runConDecl_ :: ConDecl -> ([C_TopLevel], Env)
runConDecl_ c = runState (evalConDecl c) initialEnv

runFunDef_ :: [FunDef] -> ([C_TopLevel], Env)
runFunDef_ c = runState (evalFunDef c) initialEnv

runConDecl = fst . runConDecl_

runProgram_ :: Program -> MonStack [C_TopLevel]
runProgram_ (Program conDecls defs) = do
  cs <- mapM evalConDecl conDecls
  fs <- evalFunDef defs
  mainFunc <- mainFunction (concat cs ++ fs)
  return $ concat cs ++ fs ++ [mainFunc]
    where
      mainFunction topLevels = generateFunction "main_function" stmts []
          where
            stmts = return $ funcFormatter "ref" "main_function" [] body
            allInits = topLevels ^.. traverse._C_Fun._1.filtered (\x -> x /= "init_function_main_" && startsWith "init_" x)
            body = map (st . flip funCall []) allInits ++ [st $ funCall "main_" [bracketInit "ref" []]]
        
runProgram = fst . flip runState initialEnv . runProgram_

toStatements :: [C_TopLevel] -> [Statement]
toStatements top = imps ++ vars ++ structs ++ funs
    where
      funs = top ^. traverse._C_Fun._2
      structs = top ^. traverse._C_Struct._2
      vars = top ^. traverse._C_Var._2
      imps = top ^. traverse._C_Import

renderProgram :: [Statement] -> String
renderProgram = unlines

toProgram = renderProgram . toStatements . runProgram



               
initialEnv = Env M.empty Nothing 0 M.empty [] []


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
Environment:
Need a mapping from the top-level function names, to the function info table representation
Need to store the map from var name to integer key
Need to store the current function name for generating case/thunk continuation names
Need to provide a way to get a fresh variable name that is unused
Need a mapping from constructor names to the constructor info table

Need to also store a list of functions to be genned and the deps they have.

Constructor definitions must be pre-processed seperately


(the continuation needs to come before the place where the case is used. maybe just prepended -
will need to save the state somehow - maybe add a specific comment in the header.
The deps should be acyclic, Might be a good idea to compile functions as an individual


Need to generate a makefile as well.

-}

-- Probably a source of bugs - we discard the type information here (just bad design really)
generateFunction :: String -> MonStack [Statement] -> [(Type, Arg)] -> MonStack C_TopLevel
generateFunction name fun_stmts args = do
  funMap.at name ?= funDescr
  curFun .= Just name
  stmts <- fun_stmts
  curFun .= Nothing
  funProtos %= (C_Var name [st (takeWhile (/= '{') (stmts !! 0))]:)
  return (C_Fun name stmts)
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

freshIntStream :: Int -> MonStack [Int]
freshIntStream z = do
  st <- get
  result <- stream
  put st
  freshNameSource += z
  return (take z result)
      where
        stream = (:) <$> freshInt <*> stream
  

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
              location = s "($$ + $$)" [castPtr "char" pap, offs]

knownFunction :: String -> MonStack Bool
knownFunction f = use funMap >>= return . M.notMember f

knownAndSaturated :: String -> Int -> MonStack Bool
knownAndSaturated f as = do
  args <- use (funMap.ix f.finfArgs)
  return $ (length args) == as

pushArgsFromLayoutInfo :: String -> [Atom] -> [Statement]
pushArgsFromLayoutInfo infoTable args = foldMap genPushes . reverse $ zip [0..] args
    where
      genPushes (index, V x) = ifSt cond [funCall "push_ptr" [x]] [[funCall "push_int" [x]]]
          where
            (.) = structAccess
            cond = arrayIndex (infoTable."layout"."entries") (show index)."pointer"

pushArgTypes :: [Arg] -> [Atom] -> [Statement]
pushArgTypes argTypes args = zipWith toPushInstr args argTypes
      where
        toPushInstr (V x) (_, Boxed) =  s "push_ptr($$);" [x]
        toPushInstr (V x) (_, Unboxed) =  s "push_int($$);" [x]
        toPushInstr (L i) (_, Unboxed) =  s "push_int($$);" [show i]
         
pushFunArgs :: String -> [Atom] -> MonStack [Statement]
pushFunArgs fun args = do
  argTypes <- use (funMap.ix fun.finfArgs)
  return $ pushArgTypes argTypes args
                               
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
        Boxed -> (arrayIx ..= structValue (fields "true" "sizeof(ref)"), s "$$ + sizeof(ref)" [offset])
        Unboxed -> (arrayIx ..= structValue (fields "false" "sizeof(int)"), s "$$ + sizeof(int)" [offset])
  where
    arrayIx = arrayIndex "layout_entries" ix
    structValue = bracketInit "arg_entry"
    fields isPointer argSize = [("size", argSize), ("pointer", isPointer), ("offset",  offset)]

imports = C_Import [
           includeSys "stdlib.h",
           includeSys "stdio.h",
           includeSys "assert.h",                      
           includeUser "stg/plus_int/static.h",
           includeUser "stg/bindings.h",
           includeUser "containers/mmanager.h",
           includeUser "stg/heap.h"
          ]


evalFunDef :: [FunDef] -> MonStack [C_TopLevel]
evalFunDef prog = do
  x <- mapM generateTopLevelDefn prog
  fun_prototypes <- use funProtos
  defereds <- use deferred
  return $ imports:fun_prototypes ++ defereds ++ concat x
    where
      generateTopLevelDefn (name, FUNC (Fun args e))
          = do
        let slowEntryPointDecl = C_Fun slow_entry_name
            info_struct_name = s "$$_info_table" [name]
            infoTableVar = C_Var info_struct_name [decl "info_table" info_struct_name]
        infoTable <- generateFunction info_table_name info_table_initializer_stmts []
        fastEntry <- generateFunction fast_entry_name fast_entry_point (map toCType args)
        slowEntry <- generateFunction slow_entry_name slow_entry_point []
        if name == main_entry_point
        then return [fastEntry]
        else return $ [infoTableVar, fastEntry, slowEntry, infoTable]
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
            functionStruct = bracketInit "fun" [("slow_entry_point", slow_call_name name), ("arity", show $ length args)]

            (slow_entry_name, slow_entry_point) = (slow_call_name name, generateSlowCall fast_entry_name name . map (\x -> (V . fst $ x, snd x)) $ args)
--            fastEntryPointDecl = C_Fun fast_entry_name fast_entry_point []
            fastEntryPointDescr = FInf fast_entry_name (length args) args
            fast_entry_name = if name == main_entry_point
                              then name
                              else fast_call_name name
            fast_entry_point = funcFormatter "ref" (fast_entry_name) fastArgs <$> body
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

generateSlowCall :: String -> String -> [(Atom, ValueType)] -> MonStack [String]
generateSlowCall fastName name args = do
  let genIfCase as = ifSt cond <$> genPapForArgs fastName as <*> pure [] where cond = funCall "arg_satisfaction_check" [c_sum . map (toSize . snd) $ as]
  elseStmts <- mapM genIfCase . tail . inits $ args
  let body = ifSt argSatisfactionCondition (generateFastCallFromArgsOnStack name args) (elseStmts ++ [[assert "false"]])
  return $ funcFormatter "ref" (slow_call_name name) [("ref", "null")] body
   where
     argSatisfactionCondition = funCall "arg_satisfaction_check" [argSize]
     argSize = c_sum (map (toSize . snd) args)
     generateFastCallFromArgsOnStack name args
         = map declare_var_type args
           ++ map pop_instr args
           ++ [returnSt (funCall (fast_call_name name) [commaSep . map (unwrap . fst) $ args])]
     unwrap (V x) = x


-- genPapForArgs :: String -> String -> [(Atom, ValueType)]
genPapForArgs name args  = do
   initArgs <- initArgs_
   return $ prepareArgsFromStack ++ initStructTable ++ declareNewPap:initArgs ++  [arrayIndex "pap_" 0 ..= "pap_info", returnSt "pap_ref"]
        where
          argSize = c_sum (map (toSize . snd) args)          
          -- the pap consists of the arg size + the info pointer
          declareNewPap = newRefMacro "pap_ref" "void**" (s "$$ + sizeof(void*)" [argSize]) "pap_"
          initStructTable = [declInit "struct info_table*" "pap_info" (deref $ castPtr "struct info_table*" (funCall "new" ["sizeof(info_table)"])),
                             ptrAccess "pap_info" "type" ..= "4",
                             ptrAccess "pap_info" "extra.pap_info" ..= infoTableStruct]
          infoTableStruct = bracketInit "struct pap" [("info_ptr", "pap_info"), ("size", "1")]
          prepareArgsFromStack = map declare_var_type papArgs ++ map pop_instr papArgs
          papArgs_ = fst . unzip $ papArgs
          papArgs = args
          initArgs_ = assignPapAtoms "pap_" name papArgs_

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
  funcDecl <- generateFunction name (funcFormatter "void" name args <$> body) args
  return (funcDecl:structDecls)
    where
      info_table_name c = s "$$_info_table" [c]
      conMapUpd mp c = M.insert (conName c) c mp
      structDecls = map toStructDecl cons
      toStructDecl (ConDefn conName _ fields) = C_Struct conName (decl "struct info_table" (info_table_name conName):typedef conName fields [("info_table*", "info_ptr")])
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
                              ("con_num", show tag)]
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
    generateFunction name statements (args & traverse._2 %~ (,Boxed))
  where
    thunkBody name = case es of
        -- For this case we push a case frame and then a 'fake' update frame that restores su and returns the arg that the update frame was called with 
        [AltForce x e] -> undefined -- TODO Need to handle this case.
        alts -> (++)
                <$>
                pure [
                  bindingMacro var_ref "void**" var_name var_key "bindings",
                  declInit "info_table*" info_table (deref (castPtr "info_table*" var_name))
               ] <*>
               (ifSt conCase <$> (fmap concat . mapM caseIf $ alts) <*>
                pure [thunkCase name])
    conCase = s "$$->type == 1" [info_table]    
    var_key = show . al $ M.lookup var_name bindings
    thunkCase name = [
       assert (s "$$ == $$" [ptrAccess info_table "type", "5"]),
       returnSt (funCall "thunk_continuation" [var_ref, name, "bindings", var_key, var_ref])
     ]
    caseIf (AltCase conName freeVars exp) = do
            [conDefn] <- uses (conMap.ix conName) (:[])
            let expectedConNum = conTag conDefn
                cond = s "$$ == $$" [actualConNum, show expectedConNum]
            ifBody <- do
               conInnerName <- freshName
               intStream <- freshIntStream (length . conFields $ conDefn)
               let init_con_struct = declInit (s "$$*" [conName]) conInnerName conCasted
                   (_, valTypes) = unzip $ conFields conDefn
                   freeVarBindings = zip (zip freeVars valTypes) intStream
                   bindings' = foldl updBindings bindings freeVarBindings
                   bindCaseStmts = zip (conFields conDefn) intStream >>= genBindConVarStmts
                   updBindings mp ((name, _), i) = M.insert name i mp
                   genBindConVarStmts ((field_name, value_type), num)
                       = case value_type of
                           Unboxed -> [newRefMacro tmp_var "int*" "sizeof(int)" tmp_var_val, deref tmp_var_val ..= con_field_val, putBinding tmp_var num]
                           Boxed -> [declInit "ref" tmp_var con_field_val, putBinding tmp_var num]
                            -- need to actually assign the new variables from conInnerName, then need to generate statements to bind the new names to the new ints
                       where
                         tmp_var = to_temp_var num
                         tmp_var_val = s "$$_val" [tmp_var]
                         con_field_val = ptrAccess conInnerName field_name
                                         
               case_alt_stmts <- eval bindings' exp
               
               return $ init_con_struct: bindCaseStmts ++ case_alt_stmts

            return $ ifSt cond ifBody [[st $ funCall "assert" ["false"]]]
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
  generateFunction funName fun_stmts [("ref", ("thunk_ref", Boxed))]
    where
      body = (declInit "hash_map*" "bindings" "THUNK_GET_BINDINGS(thunk_ref)":) <$> eval bindings e

-- would be interesting to implement printf in the Real World State monad
evalPrimop :: String -> Bindings -> Primop -> MonStack [String]             
evalPrimop res bindings (Primop "print_int" [L x]) = return [
                                            st $ funCall "printf" ["%d\n", show x],
                                            decl "ref" res
                                           ]
evalPrimop res bindings (Primop "print_int" [V x]) = do
  tmp <- freshName
  let tmp_ref = s "$$_ref" [tmp]
      [var_key] = bindings ^.. ix x
      assignPrintVar = bindingMacro tmp_ref "int*" tmp (show var_key) "bindings"
      printStatement = st $ funCall "printf" ["\"%d\\n\"", deref tmp]
  return [assignPrintVar, printStatement, decl "ref" res] 
evalPrimop res bindings (Primop "+#" [L x, L y]) = return [declInit "int" res $ show (x + y)]
evalPrimop res bindings (Primop "+#" [V x, V y]) = do
  if length (bindings ^.. ix x) == 0
  then error . show $ bindings
  else return ()
  let [xkey] = bindings ^.. ix x
      [ykey] = bindings ^.. ix y
      x_ref = s "$$_ref" [x]
      y_ref = s "$$_ref" [y]
      initX = bindingMacro x_ref "int*" x (show xkey) "bindings"
      initY = bindingMacro y_ref "int*" y (show ykey) "bindings"
  return $ [
        initX,
        initY,
        declInit "int" res (s "$$ + $$" [deref x, deref y])
   ]
    


eval :: Bindings -> Expression -> MonStack [String]
eval bindings c@(Case deb _) = do
  func_prefix <- freshName
  let name = s "$$_$$" [func_prefix, "cont"]
  case_cont <- generateCaseCont name bindings c
  deferred %= (++[case_cont])
  return [returnSt (funCall name ["bindings"])]


-- (void**)get_binding()
{-
If it's a function call thats not to a known function we need to figure out what type it is - thunk, func, pap and then push the args and generate a slow call.
For a pap and fun, since we don't know at compile time which function has been called and what type the args are, we generate a bunch of case logic from the layout description of the function info table
-}
eval bindings (FuncCall fun args) = do
  let fastFun = fast_call_name fun
  knownAndSaturatedCond <- knownAndSaturated fastFun (length args)
  knownCond <- knownFunction fastFun
  if knownAndSaturatedCond
  then do
    let toArgInit (V x) = [decl "ref" x, getBinding (al $ M.lookup x bindings) x]
        toArgInit (L _) = [] -- pass them directly
        initArgsFromBindings = args >>= toArgInit
    return $ initArgsFromBindings ++ [returnSt $ funCall fastFun (map extractArgsToFunArgs args)]
  else if knownCond
       then do
         tmp <- freshName
         pushStmts <- pushFunArgs fastFun (reverse args)
         return $ pushStmts ++ [decl "ref" tmp, returnSt $ funCall (slow_call_name fun) [tmp]]
  else do
    tmp <- freshName
    funArgPushes <- undefined
    let [funKey] = bindings ^.. ix fun
        fun_ref = s "$$_ref" [fun]
        initFun = bindingMacro fun_ref "void**" fun (show funKey) "bindings"
        initFunTable = declInit "info_table" infoTable
        funBody = funArgPushes ++ [decl "ref" tmp, returnSt $ funCall (funSlowEntry infoTable) [tmp]]
        papArgPushes = pushArgsFromLayoutInfo papFunInfoTable args                  
        papBody = papArgPushes ++ [funCall "unroll_pap" [fun], decl "ref" tmp, returnSt $ funCall papSlowEntry [tmp]]
        blackholeCase = ifSt blackholeCheck [assert "false"] [] where blackholeCheck = s "$$.type == 6" [infoTable]
        funcCase = ifSt funcCheck funBody [] where funcCheck = s "$$.type == 0" [infoTable]
        papCase = ifSt papCheck papBody [] where papCheck = s "$$.type == 4" [infoTable]
        thunkCase = undefined
    return $ funcCase ++ papCase ++ thunkCase
    where
      infoTable = s "$$_info" [fun]
      funSlowEntry funTable = structAccess (structAccess (structAccess funTable "extra") "function") "slow_entry_point"
      papFunInfoTable =  deref $ structAccess (structAccess (structAccess infoTable "extra") "pap_info") "info_ptr"
      papSlowEntry = funSlowEntry papFunInfoTable
      
                                                       

{-
   let x' = THUNK (f x) in e

   ref thunk1_ref = create_thunk(bindings, map_thunk1);
   put_binding(bindings, 4, thunk1_ref);

-}
eval bindings (Let var obj e) = do
                     updateKey <- freshInt
                     let bs = [putBinding thunk_ref_name updateKey]
                     thunkObj <- evalObject bindings var thunk_ref_name obj
                     rest <- eval (M.insert var updateKey bindings) e
                     return $ thunkObj ++ bs ++ rest
    where
      thunk_ref_name = s "$$_ref" [var]


eval bindings (Atom (L x)) = return [returnSt $ show x]
eval bindings (Atom (V x)) = return [returnSt $ s "$$_ref" [x]]
eval bindings (Atom (P p)) = do
  tmp <- freshName
  st <- evalPrimop tmp bindings p
  return $ st ++ [returnSt tmp]


evalObject :: M.Map String Int -> String -> String -> Object -> MonStack [String]
evalObject bindings obj_name obj_ref_name t@(THUNK e) = do
  thunk_cont <- freshName
  deferred_thunk <- generateThunkCont bindings thunk_cont t
  deferred %= (++[deferred_thunk])
  -- ugh, eval expects the object as a ref
  return [declInit "ref" obj_ref_name $ funCall "create_thunk" ["bindings", thunk_cont]]
{-
   Cons x' f'
//
   NEW_REF(cons_ref, Cons*, sizeof(Cons), cons)
   cons->info_ptr = &cons_info_table;
   cons->value = thunk1_ref;
   cons->next = thunk2_ref;
-}
evalObject bindings obj_name obj_ref_name (CON (Con c atoms)) = do
  [constrDefn] <- uses (conMap.ix c) (:[])
  let fields = fst . unzip . conFields $ constrDefn
  assignFields <- zipWithM assignField fields atoms
  return $ [newRefMacro ref_name (s "$$*" [c]) (s "sizeof($$)"[c]) val_name] ++ assignInfoPtr:concat assignFields
    where
      val_name = obj_name
      c_info_table = s "$$_info_table" [c]
      ref_name = obj_ref_name
      assignField f (V x) = return [ptrAccess val_name f ..= x]
      assignField f (L x) = return [ptrAccess val_name f ..= show x]
      assignField f (P p) = do
        tmp <- freshName
        sts <- evalPrimop tmp bindings p
        return $ sts ++ [ptrAccess val_name f ..= tmp]
      assignInfoPtr = ptrAccess val_name "info_ptr"  ..= (s "&$$" [c_info_table])
evalObject _ _ _ BLACKHOLE = error "<loop>"
evalObject _ _ _ (FUNC _) = error "I dont support lambdas yet."
-- TODO example is in list.c:60 (this might be wrong
evalObject bindings obj_name obj_ref_name (PAP (Pap fun as)) = undefined -- do
--  return [st$ funCall "unroll_pap" fun, decl "ref" "null", returnSt $ finfo ]
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
