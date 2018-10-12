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
import Data.Monoid
import Bookeeping

debug = undefined


-- TODO : Tail call optimization
-- TODO : Growing the stack - walk the su frames and update them (Instead its easier to store su and stack_pointer as relative offsets from the stack top)
-- TODO : Create a parser, also have a pass that ensures all names are globally unique
-- TODO: Is there a nice way of implementing a lazy stateful infinite stream that doesn't highjack the state (freshIntStream could be infinite)

{-
  Buglist:
   - pushArgsFromLayout is incorrect: it starts from index 0 when instead it should take `len args` from a list of [maxNumArgsOfFunc - 1 .. 0]
   - we may not push and pop paps in the correct order - fair warning


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
      mainFunction topLevels = generateFunction Nothing "main_function" stmts []
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

initialEnv = Env M.empty Nothing 0 M.empty [] [] M.empty M.empty


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
generateFunction :: Maybe String -> String -> MonStack [Statement] -> [(Type, Arg)] -> MonStack C_TopLevel
generateFunction tableName name fun_stmts args = do
  funMap.at name ?= funDescr
  savedScope <- use liveVars
  liveVars .= M.fromList (map (\p -> (fst . snd $ p, fst p)) args)
  curFun .= Just name
  stmts <- fun_stmts
  curFun .= Nothing
  liveVars .= savedScope
  funProtos %= (C_Var name [st (takeWhile (/= '{') (stmts !! 0))]:)
  return (C_Fun name stmts)
      where
        funDescr = FInf tableName name (length args) (map snd args) layoutEnts
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



generatePapSize :: String -> Int -> MonStack String
generatePapSize fun argLen = do
  -- you want the first n arguments.
  args <- use (funMap.ix fun.finfArgs)
  return . c_sum . map (toSize . snd)  . take argLen $ args

{-
     *(ref*)(pap + 1) = one_ref;
-}
assignPapAtoms giveRefsSuffix pap fun atoms = do
  layout <- use (funMap.ix fun.finfLayout)
  return $ zipWith toAssignment layout atoms
      where
        toAssignment (LayoutEntry _ isPtr offs) a
            = if not isPtr
              then deref (castPtr "int" location) ..= atomToVal a
              else deref (castPtr "ref" location) ..= (if giveRefsSuffix
                                                       then s "$$_ref" [atomToVal a]
                                                       else atomToVal a)
            where
              atomToVal (L i) = show i
              atomToVal (V v) = v
              location = s "($$ + $$)" [castPtr "char" pap, offs]

knownFunction :: String -> MonStack Bool
knownFunction f = use funMap >>= return . M.member f

knownAndSaturated :: String -> Int -> MonStack Bool
knownAndSaturated f as = do
  args <- use (funMap.ix f.finfArgs)
  return $ (length args) == as

pushArgsFromLayoutInfo :: Bindings -> String -> [Atom] -> MonStack [Statement]
pushArgsFromLayoutInfo bindings infoTable args
    = fmap concat . traverse genPushes . reverse $ zip [0..] args
    where
      genPushes (index, L x) = return [push_instr (L x, Unboxed)]
      genPushes (index, V x)
          = do
        declare_xref <- decl "ref" (refname x)
        bindingStmt <- bindingMacro (refname x) "int*" x (show updateKey) "bindings"
        return $ ifSt cond [declare_xref, getBinding updateKey (refname x), push_instr (V (refname x), Boxed)]
                   [[bindingStmt, push_instr (V (deref x), Unboxed)]]
          where
            updateKey = maybe (error "#1") id $ M.lookup x bindings
            refname x = s "$$_ref" [x]
            (.) = structAccess
            cond = arrayIndex (infoTable."layout"."entries") index."pointer"

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
           includeUser "stg/heap.h",
           includeUser "data/string_.h"                       
          ]



evalFunDef :: [FunDef] -> MonStack [C_TopLevel]
evalFunDef prog = do
  x <- mapM generateTopLevelDefn prog
  defereds <- whileM ((not . null) <$> use deferred) (do
                                                       d <- use deferred
                                                       deferred .= []
                                                       mapM id d
                                                     )
  fun_prototypes <- use funProtos
  return $ imports:fun_prototypes ++ concat defereds ++ concat x
    where
      generateTopLevelDefn (name, FUNC (Fun args e))
          = do
        let slowEntryPointDecl = C_Fun slow_entry_name
            info_struct_name = s "$$_info_table" [name]
        funcHeapObjectRef <- C_Var funcHeapObjectName . (:[]) <$> decl "ref" funcHeapObjectName
        infoTableVar <- C_Var info_struct_name . (:[]) <$> decl "info_table" info_struct_name                    
        infoTable <- generateFunction Nothing info_table_name info_table_initializer_stmts []
        fastEntry <- generateFunction (Just info_struct_name) fast_entry_name fast_entry_point (map toCType args)
        slowEntry <- generateFunction (Just info_struct_name) slow_entry_name slow_entry_point []
        if name == main_entry_point
        then return [fastEntry]
        else return $ [infoTableVar, funcHeapObjectRef, fastEntry, slowEntry, infoTable]
          where
            funcHeapObjectName = s "$$_ref" [name]
            (initializeLayoutEntries, offsets) = unzip $ map init_arg_entry $ zip [0..] $ zip ("0":offsets) args
            info_table_function = Fun [] 
            (info_table_name, info_table_initializer_stmts) = (init_func_name, funcFormatter "void" init_func_name [] <$> body)
                where
                  init_func_name = s "init_function_$$" [name]
                  body = do
                    let initializeHeapObjRef = st $ funCall "new_ref" ["sizeof(void*)", reference funcHeapObjectName]
                        referenceInfoTableFromHeapObj = deref ((castPtr "info_table*") (funCall "get_ref" [funcHeapObjectName])) ..= reference (funInfoTableName name)

                    allocateLayoutObject <- declInit "arg_entry*" "layout_entries" $ castPtr "arg_entry" $ funCall "new" $ [s "sizeof(arg_entry)*$$" [show $ length args]]
                    return $ allocateLayoutObject:initializeLayoutEntries ++ [funInfoTableName name ..= info_table_struct] ++ [initializeHeapObjRef, referenceInfoTableFromHeapObj]
                  layout = bracketInit "struct layout" [("num", "2"), ("entries", "layout_entries")]
                  info_table_struct = bracketInit "info_table" $
                            [("type", "0"),
                             ("extra", functionStruct),
                             ("layout", layout)
                            ]
            functionStruct = bracketInit "fun" [("slow_entry_point", slow_call_name name), ("arity", show $ length args)]

            (slow_entry_name, slow_entry_point) = (slow_call_name name, generateSlowCall fast_entry_name name . map (\x -> (V . fst $ x, snd x)) $ args)
--            fastEntryPointDecl = C_Fun fast_entry_name fast_entry_point []
            fast_entry_name = if name == main_entry_point
                              then name
                              else fast_call_name name
            fast_entry_point = funcFormatter "ref" (fast_entry_name) fastArgs <$> body
                where
                  fastArgs = map f args
                  newBindings' = zip args [0..]
                  newBindings = let g ((a, _), b) = (a, b) in map g newBindings'
                  genBindingSt ((name, Boxed), b) = return [putBinding name b]
                  genBindingSt ((name, Unboxed), b) = do
                    let refn = s "$$_ref" [name]
                        initRef = st $ funCall "new_ref" ["sizeof(int)", reference refn]
                        initRefVal = deref (castPtr "int" (funCall "get_ref" [refn])) ..= name
                    d <- decl "ref" refn
                    return [d, initRef, initRefVal, putBinding refn b]
                  body = do
                    stringBindings %= M.union (M.fromList newBindings)
                    bs <- initBindings
                    bindingSts <- traverse genBindingSt newBindings'
                    _e <- eval e
                    return $ bs ++ concat bindingSts ++ _e
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
  let genIfCase as = do
        declStackArgs <- mapM declare_var_type as
        let prepareArgsFromStack = declStackArgs ++ map pop_instr as
        body <- genPapForArgs False "pap_" fastName as
        return $ ifSt cond (prepareArgsFromStack ++ body) []
          where
            cond = funCall "arg_satisfaction_check" [c_sum . map (toSize . snd) $ as]


  elseStmts <- mapM genIfCase . tail . inits $ args
  fastCall <- generateFastCallFromArgsOnStack name args               
  let body = ifSt argSatisfactionCondition fastCall (elseStmts ++ [[assert "false"]])
  return $ funcFormatter "ref" (slow_call_name name) [("ref", "null")] body
   where
     argSatisfactionCondition = funCall "arg_satisfaction_check" [argSize]
     argSize = c_sum (map (toSize . snd) args)
     generateFastCallFromArgsOnStack name args = do
                                        decls <- mapM declare_var_type args
                                        return $ decls
                                                 ++ map pop_instr args
                                                 ++ tailCall (fast_call_name name) (map unwrap args)
     unwrap (V x, z) = (x, z)


genPapForArgs :: Bool -> String -> String -> [(Atom, ValueType)] -> MonStack [Statement]
genPapForArgs giveRefsSuffix pap_name name args  = do
   initArgs <- initArgs_
   [Just fun_info_name] <- uses (funMap.ix name.finfTableName) (:[])
   let infoTableStruct = bracketInit "struct pap" [("info_ptr", reference fun_info_name), ("size", "1")]
   initStructTable <- do
         infoTableDeclInit <- declInit "struct info_table*" pap_info_name (castPtr "struct info_table" (funCall "new" ["sizeof(info_table)"]))
         return [infoTableDeclInit,
                 ptrAccess pap_info_name "type" ..= "4",
                 ptrAccess pap_info_name "extra.pap_info" ..= infoTableStruct]
   declareNewPap <- newRefMacro pap_ref_name "void**" (s "$$ + sizeof(void*)" [argSize]) pap_name                    
   return $ initStructTable ++ declareNewPap:initArgs ++  [arrayIndex pap_name 0 ..= pap_info_name]
        where
          pap_ref_name = s "$$_ref" [pap_name]
          pap_info_name = s "$$_info" [pap_name]
          argSize = c_sum (map (toSize . snd) args)          
          -- the pap consists of the arg size + the info pointer

          papArgs_ = fst . unzip $ papArgs
          papArgs = args
          initArgs_ = assignPapAtoms giveRefsSuffix pap_name name papArgs_

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
  funcDecl <- generateFunction Nothing name (funcFormatter "void" name args <$> body) args
  structDecls <- mapM toStructDecl cons
  return $ funcDecl:structDecls
    where
      info_table_name c = s "$$_info_table" [c]
      conMapUpd mp c = M.insert (conName c) c mp
      toStructDecl (ConDefn conName _ fields) = do
                                        structDeclaration <- decl "struct info_table" (info_table_name conName)
                                        return $ C_Struct conName (structDeclaration:typedef conName fields [("info_table*", "info_ptr")])
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
                              ("con_num", show tag),
                              ("con_name", bracketInit "string_" [
                                                ("char_arr", s "\"$$\"" [conName]),
                                                ("length", show . length $ conName)
                                              ])
                             ]
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
generateCaseCont :: String -> Expression -> MonStack C_TopLevel
generateCaseCont name (Case (V var_name) es) = do
    let statements = funcFormatter returnType name args <$> thunkBody name
    generateFunction Nothing name statements (args & traverse._2 %~ (,Boxed))
  where
    thunkBody name = case es of
        -- For this case we push a case frame and then a 'fake' update frame that restores su and returns the arg that the update frame was called with 
        [AltForce x e] -> do
          bindings <- use stringBindings
          var_key <- freshName
          let var_key_num = show . alX $ M.lookup var_name bindings
              isAThunk = s "$$->type == 5" [info_table]
          varKeyNumAssign <- declInit "int" var_key var_key_num
          rest <- eval e
          varKeyStmts <- getVarKey var_key_num
          return $ varKeyStmts
                     ++ varKeyNumAssign:ifSt isAThunk (thunkCase var_key name) [rest]
        alts -> do
          bindings <- use stringBindings
          var_key <- freshName                      
          let var_key_num = show . maybe (error $ s "GenCaseCont $$" [var_name]) id $ M.lookup var_name bindings
          varKeyNumAssign <- declInit "int" var_key var_key_num
          caseIfAlts <- mapM caseIf alts
          varKeyStmts <- getVarKey var_key_num
          return $ varKeyStmts ++ varKeyNumAssign:ifSt conCase (concat caseIfAlts) [thunkCase var_key name]
    conCase = s "$$->type == 1" [info_table]    
    getVarKey var_key = do
      declareInfoTable <- declInit "info_table*" info_table (deref (castPtr "info_table*" var_name))      
      bindingStmt <- bindingMacro var_ref "void**" var_name var_key "bindings"
      return [bindingStmt, declareInfoTable]
    thunkCase var_key name = assert (s "$$ == $$" [ptrAccess info_table "type", "5"]):
                             tailCall "thunk_continuation" [(var_ref, Boxed), (name, Boxed), ("bindings", Boxed), (var_key, Unboxed), (var_ref, Boxed)]
    caseIf (AltCase conName freeVars exp) = do
            [conDefn] <- uses (conMap.ix conName) (:[])
            let expectedConNum = conTag conDefn
                cond = s "$$ == $$" [actualConNum, show expectedConNum]
            ifBody <- do
               conInnerName <- freshName
               intStream <- freshIntStream (length . conFields $ conDefn)
               init_con_struct <- declInit (s "$$*" [conName]) conInnerName conCasted                            
               let (_, valTypes) = unzip $ conFields conDefn
                   freeVarBindings = zip (zip freeVars valTypes) intStream
               let updBindings ((name, _), i) = stringBindings.at name ?= i
                   genBindConVarStmts ((field_name, value_type), ((case_var_name,_), num))
                       = case value_type of
                           Unboxed -> newRefMacro tmp_var_ref "int*" "sizeof(int)" tmp_var >>= return . (:[deref tmp_var ..= con_field_val, putBinding tmp_var_ref num])
                           Boxed -> declInit "ref" tmp_var_ref con_field_val >>= return . (:[putBinding tmp_var_ref num])
                            -- need to actually assign the new variables from conInnerName, then need to generate statements to bind the new names to the new ints
                       where
                         tmp_var_ref = s "$$_ref" [tmp_var]
                         tmp_var = case_var_name
                         con_field_val = ptrAccess conInnerName field_name
               bindCaseStmts <- fmap concat . traverse genBindConVarStmts $ zip (conFields conDefn) freeVarBindings
               mapM_ updBindings freeVarBindings
               case_alt_stmts <- eval exp
               
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
generateThunkCont funName (THUNK e)
    = do
  let fun_stmts = funcFormatter "ref" funName [("ref", "thunk_ref")] <$> ((:) <$> declInit "hash_map*" "bindings" "THUNK_GET_BINDINGS(thunk_ref)" <*> eval e)
  generateFunction Nothing funName fun_stmts [("ref", ("thunk_ref", Boxed))]


-- would be interesting to implement printf in the Real World State monad
evalPrimop :: String -> Primop -> MonStack [String]             
evalPrimop res (Primop "print_int" [L x]) = do
  resDecl <- decl "ref" res
  return $ [st $ funCall "printf" ["%d\n", show x], resDecl]
evalPrimop res (Primop "exception" []) = do
  resDecl <- decl "ref" res
  return [st $ funCall "assert" ["false"], resDecl]
evalPrimop res (Primop "print_int" [V x]) = do
  bindings <- use stringBindings
  tmp <- freshName
  let tmp_ref = s "$$_ref" [tmp]
      res_val = s "$$_val" [res]
      [var_key] = bindings ^.. ix x
      printStatement = st $ funCall "printf" ["\"%d\\n\"", deref tmp]
  assignPrintVar <- bindingMacro tmp_ref "int*" tmp (show var_key) "bindings"
  resDecl <- decl "ref" res
  unitResultStmts <- evalObject res_val res (CON $ Con "Unit" [])
  return $ [assignPrintVar, printStatement] ++ unitResultStmts
evalPrimop res (Primop op [L x, L y]) = (:[]) <$> declInit "int" res (show $ maybe (error op) (\f -> f x y) $ M.lookup op binPrimops)
evalPrimop res (Primop op [V x, V y]) = do
  bindings <- use stringBindings
  if length (bindings ^.. ix x) == 0
  then error . show $ bindings
  else return ()
  let [xkey] = bindings ^.. ix x
      [ykey] = bindings ^.. ix y
      x_ref = s "$$_ref" [x]
      y_ref = s "$$_ref" [y]
  initX <- bindingMacro x_ref "int*" x (show xkey) "bindings"
  initY <- bindingMacro y_ref "int*" y (show ykey) "bindings"
  resDecl <- decl "int" res
  return $ resDecl : newScope [
        initX,
        initY,
        res ..= s "$$ $$ $$" [deref x,
                              maybe (error op) id $ M.lookup op cOps,
                              deref y]
   ]
evalPrimop res (Primop op [V x, L y]) = do
  bindings <- use stringBindings
  if length (bindings ^.. ix x) == 0
  then error . show $ bindings
  else return ()
  let [xkey] = bindings ^.. ix x
      x_ref = s "$$_ref" [x]
  initX <- bindingMacro x_ref "int*" x (show xkey) "bindings"
  resDecl <- decl "int" res
  return $ resDecl : newScope [
        initX,
        case op of
          "==" -> res ..= funCall "int_equals" [x, show y]
          op_ -> res ..= s "$$ $$ $$" [deref x,
                                       maybe (error op) id $ M.lookup op_ cOps,
                                       show y]
   ]
    


eval :: Expression -> MonStack [String]
eval c@(Case (P p) as) = do
  tmp <- freshName
  sts <- evalPrimop tmp p
  ((sts++) . concat) <$> mapM (genPrimCaseAlt tmp) as
      where
        {-
          We assume that primop case expressions can only evaluate to unlifted
          (in our case only ints).
         -}
        genPrimCaseAlt t (AltCase x [] e) = ifSt tEqualsX <$> bodyEvaled <*> pure []
            where
              tEqualsX = s "$$ == $$" [t, x]
              bodyEvaled = eval e
        genPrimCaseAlt t (AltForce x e) = (:) <$> assignTmpToX <*> eval e
            where assignTmpToX = declInit "int" x t

eval c@(Case _ _) = do
  func_prefix <- freshName
  let name = s "$$_$$" [func_prefix, "cont"]
  deferred %= (++[generateCaseCont name c])
  return $ tailCall name [("bindings", Boxed)]


-- (void**)get_binding()
{-
If it's a function call thats not to a known function we need to figure out what type it is - thunk, func, pap and then push the args and generate a slow call.
For a pap and fun, since we don't know at compile time which function has been called and what type the args are, we generate a bunch of case logic from the layout description of the function info table
-}
eval (FuncCall fun args) = do
  bindings <- use stringBindings
  let fastFun = fast_call_name fun
  knownAndSaturatedCond <- knownAndSaturated fastFun (length args)
  knownCond <- knownFunction fastFun
  fMap <- use funMap
  _debug <- uses (funMap.ix fun.finfArgs) (:[])
  let argTypes = if null _debug then error $ s "$$, $$: $$" [fun, fastFun, show fMap] else head _debug
  if knownAndSaturatedCond
  then do
    let toArgInit (a@(V x), (_, Unboxed)) = return ([], (a, Nothing))
        toArgInit (a@(V x), (_, Boxed)) = do
                                        x_decl <- decl "ref" x
                                        return ([x_decl, getBinding (al $ M.lookup x bindings) x], (a, Nothing))
        toArgInit (a@(L _), _) = return ([], (a, Nothing)) -- pass them directly
        toArgInit (a@(P p), _) = do
          freshName >>= \n -> evalPrimop n p >>= \sts -> return (sts, (a, Just n))
    bs <- mapM toArgInit (zip args argTypes)
    let (initArgsFromBindings, argsWithResultVars) = unzip bs
    -- I new scope over here because at this point I don't have a clear idea of what's in scope - I should really be keeping a map of whats in scope at this point.
    return $ newScope $ concat initArgsFromBindings ++ [returnSt $ funCall fastFun (map extractArgsToFunArgs argsWithResultVars)]
  else if knownCond
       then do
         tmp <- freshName
         pushStmts <- pushFunArgs fastFun (reverse args)
         tmp_decl <- decl "ref" tmp
         return $ pushStmts ++ tmp_decl:tailCall (slow_call_name fun) [(tmp, Boxed)]
  else do
    tmp <- freshName
    Just thunkContName <- use curFun
    let asd = bindings ^.. ix fun
        fun_ref = s "$$_ref" [fun]
        funKey = if null asd then error (s "FuncCall case: $$" [fun]) else head asd
    initFun <- bindingMacro fun_ref "void**" fun (show funKey) "bindings"
    funKeyVar <- freshName
    funKeyNumAssign <- declInit "int" funKeyVar (show funKey)
    initFunTable <- declInit "info_table" infoTable (deref . deref $ castPtr "info_table*" fun)
    funArgPushes <- pushArgsFromLayoutInfo bindings infoTable args
    tmpDecl <- decl "ref" tmp
    funTmp <- freshName
    funJmpAddrStore <- declInit "void*" funTmp (castPtr "void" funSlowEntryLoc)
    let funUnknownCallStmts = unknownTailCall funTmp funSlowEntryLoc [(tmp, Boxed)]
        funBody = funArgPushes ++ tmpDecl:funJmpAddrStore:funUnknownCallStmts
    papArgPushes <- pushArgsFromLayoutInfo bindings papFunInfoTable args

    papTmp <- freshName
    papJmpAddrStore <- declInit "void*" papTmp (castPtr "void" papSlowEntry)

    let papUnknownCallStmts = unknownTailCall papTmp papSlowEntry [(tmp, Boxed)]
        papBody = papArgPushes ++ [st $ funCall "unroll_pap" [fun], tmpDecl] ++ papJmpAddrStore:papUnknownCallStmts
        blackholeCase = ifSt blackholeCheck [assert "false"] [] where blackholeCheck = s "$$.type == 6" [infoTable]
        funcCase = ifSt funcCheck funBody [papCase, thunkCase] where funcCheck = s "$$.type == 0" [infoTable]
        papCase = ifSt papCheck papBody [] where papCheck = s "$$.type == 4" [infoTable]
        thunkCase = assert (s "$$.type == $$" [infoTable, "5"]):
                    tailCall "thunk_continuation" [(fun_ref, Boxed), (thunkContName, Boxed), ("bindings", Boxed), (funKeyVar, Unboxed), (fun_ref, Boxed)]
            
    return $ initFun:funKeyNumAssign:initFunTable:funcCase
    where
      infoTable = s "$$_info" [fun]
      funSlowEntryLoc = funSlowEntry infoTable
      funSlowEntry funTable = structAccess (structAccess (structAccess funTable "extra") "function") "slow_entry_point"
      papFunInfoTable =  deref $ structAccess (structAccess (structAccess infoTable "extra") "pap_info") "info_ptr"
      papSlowEntry = funSlowEntry papFunInfoTable
      
                                                       

{-
   let x' = THUNK (f x) in e

   ref thunk1_ref = create_thunk(bindings, map_thunk1);
   put_binding(bindings, 4, thunk1_ref);

-}
eval (Let var obj e) = do
                     updateKey <- freshInt
                     let bs = [putBinding thunk_ref_name updateKey]
                     thunkObj <- evalObject var thunk_ref_name obj
                     stringBindings.at var ?= updateKey
                     rest <- eval e
                     return $ thunkObj ++ bs ++ rest
    where
      thunk_ref_name = s "$$_ref" [var]


eval (Atom (L x)) = return [returnSt $ show x]
eval (Atom (V x)) = do
  vars <- use liveVars
  let x_ref = s "$$_ref" [x]
  if M.member x_ref vars
  then return [returnSt x_ref]
  else do
    [key] <- uses (stringBindings.ix x) (:[])
    declX <- decl "ref" x_ref
    return [declX, getBinding key x_ref, returnSt x_ref]
eval (Atom (P p)) = do
  tmp <- freshName
  st <- evalPrimop tmp p
  return $ st ++ [returnSt tmp]


evalObject :: String -> String -> Object -> MonStack [String]
evalObject obj_name obj_ref_name t@(THUNK e) = do
  thunk_cont <- freshName
  deferred %= (++[generateThunkCont thunk_cont t])
  -- ugh, eval expects the object as a ref
  (:[]) <$> declInit "ref" obj_ref_name (funCall "create_thunk" ["bindings", thunk_cont])

{-
   Cons x' f'
//
   NEW_REF(cons_ref, Cons*, sizeof(Cons), cons)
   cons->info_ptr = &cons_info_table;
   cons->value = thunk1_ref;
   cons->next = thunk2_ref;
-}
evalObject obj_name obj_ref_name (CON (Con c atoms)) = do
  bindings <- use stringBindings
  
  ld <- uses (conMap.ix c) (:[])
  [constrDefn] <- if null ld then error (s "$$" [c])
                  else return ld
  let fields = fst . unzip . conFields $ constrDefn
  newRef <- newRefMacro ref_name (s "$$*" [c]) (s "sizeof($$)"[c]) val_name
  assignFields <- zipWithM assignField fields atoms
  return $ newRef:assignInfoPtr:concat assignFields
    where
      val_name = obj_name
      c_info_table = s "$$_info_table" [c]
      ref_name = obj_ref_name
      assignField f (V x) = do
        let x_ref = s "$$_ref" [x]
        x_key_ <- uses (stringBindings.ix x) (:[])
        x_ref_key_ <- uses (stringBindings.ix x_ref) (:[])
        let x_key = head $ x_ref_key_ ++ x_key_
        lv <- use liveVars
        extraStats <- if M.notMember x_ref lv
                      then (:[]) <$> bindingMacro x_ref "void**" x (show x_key) "bindings"
                      else return []
        return $ extraStats ++ [ptrAccess val_name f ..= x_ref]
      assignField f (L x) = return [ptrAccess val_name f ..= show x]
      assignField f (P p) = do
        tmp <- freshName
        sts <- evalPrimop tmp p
        return $ sts ++ [ptrAccess val_name f ..= tmp]
      assignInfoPtr = ptrAccess val_name "info_ptr"  ..= (s "&$$" [c_info_table])
evalObject _ _ BLACKHOLE = error "<loop>"
evalObject _ _ (FUNC _) = error "I dont support lambdas yet."
-- TODO example is in list.c:60 (this might be wrong)
evalObject obj_name obj_ref_name (PAP (Pap fun as)) = do
  funArgs <- use (funMap.ix fun.finfArgs)
  let argValTypes = zip as (snd $ unzip funArgs)
  genPapForArgs True obj_name fun argValTypes
    
  -- return [st$ funCall "unroll_pap" fun, decl "ref" "null", returnSt $ finfo ]
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
