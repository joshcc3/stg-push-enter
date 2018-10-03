{-# LANGUAGE LambdaCase #-}

import Types
import Utils
import CConstructs

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

generatePapSize = undefined -- generate the pap size from the atoms
assignPapAtoms pap fun atoms = undefined -- gen from env
toPrimOpArgs = undefined -- gen from env
knownAndSaturated f as = undefined -- gen from env
pushFunArgs = undefined -- gen from env


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

This should also go in a seperate file and directory and create a c and header file
Also we only deal with top-level definitions of function currently
-}
init_arg_entry (ix, (offset, (argName, argType))) = case argType of
        Boxed -> (arrayIx .= structValue (fields "true" "sizeof(ref)"), s "$$ + sizeof(ref)" [show offset])
        Unboxed -> (arrayIx .= structValue (fields "false" "sizeof(int)"), s "$$ + sizeof(int)" [show offset])
  where
    arrayIx = arrayIndex "layout_entries" ix
    structValue = bracketInit "arg_entry"
    fields isPointer argSize = [("size", argSize), ("pointer", isPointer), (".offset", show offset)]
evalProgram topLevel = topLevel >>= generateTopLevelDefn
    where
      generateTopLevelDefn (name, FUNC (Fun args e)) = info_table_initializer ++ slow_entry_point ++ fast_entry_point
          where
            info_table_initializer = funcFormatter "void" name [] body
                where
                  name = s "init_function_$$" [name]
                  body = initLayout name args ++ [funInfoTableName name .= info_table_struct]
                  initLayout name args = allocateLayoutObject:initializeLayoutEntries
                    where
                      allocateLayoutObject = declInit "arg_entry*" "layout_entries" $ castPtr "arg_entry" $ funCall "new" $ [s "sizeof(arg_entry)*$$" [show $ length args]]
                      (initializeLayoutEntries, offsets) = unzip $ map init_arg_entry $ zip [0..] $ zip ("0":offsets) args
                  layout = bracketInit "struct layout" [("num", "2"), ("entries", "layout_entries")]
                  info_table_struct = bracketInit "info_table" $
                            [("type", "0"),
                             ("extra", functionStruct),
                             ("layout", layout)
                            ]
            functionStruct = bracketInit "function" [("slow_entry_point", slow_call_name name), ("arity", show $ length args)]
            slow_entry_point = generateSlowCall name (snd . unzip $ args)
            fast_entry_point = funcFormatter "ref" (fast_call_name name) fastArgs body
                where
                  fastArgs = map f args
                  body = initBindings ++ map (uncurry putBinding) (zip [0..] (fst . unzip $ args) ) ++ eval e
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
generateSlowCall name args = funcFormatter "ref" (slow_call_name name) [("ref", "null")] body
   where
     argSatisfactionCondition = undefined -- funCall "arg_satisfaction_check" argSize
     argSize = charSeperate '+' (map toSize args)
     body = ifSt argSatisfactionCondition
              undefined -- (generateFastCallFromArgsOnStack name args)
              [elseSt]
     elseSt = undefined -- TODO create a pap
     generateFastCallFromArgsOnStack name args
         = map (decl "arg" . fst)  args ++
           map (funCall "pop_ptr" . (:[]) . reference . fst) args ++
           [returnSt (funCall (fast_call_name name) [commaSep . map fst $ args])]

{-
data List a = Cons { value :: a, next :: (List a) } | Nil

void init_list()
{
    cons_info_table = (struct info_table){ .type = 1, .extra = { .constructor = { .arity = 2, .con_num = 0 } } };

    nil_info_table = (struct info_table){ .type = 1, .extra = { .constructor = { .arity = 0, .con_num = 1 } } };

    new_ref(sizeof(Nil), &nil_value);
    ((Nil*)get_ref(nil_value))->info_ptr = &nil_info_table;
}

-}
evalConDecl (Decl typeName cons) = funcFormatter "void" name args body
    where
      name = s "init_constructors_$$" [typeName]
      args = []
      body = map generateConDefn cons
      -- TODO you also need to register that the Constructor struct gets put into the header file.
      generateConDefn (ConDefn conName tag l)
          = info_table_name  .= bracketInit "info_table" bs
            where
              info_table_name = s "$$_info_table" [conName]
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
generateCaseCont (Case (V var_name) es) = funcFormatter returnType name args $
     case es of
        -- For this case we push a case frame and then a 'fake' update frame that restores su and returns the arg that the update frame was called with 
        [AltForce x e] -> undefined -- TODO Need to handle this case.
        alts ->  [
            bindingMacro var_ref "void**" var_name var_key arg1,
            declInit "info_table*" info_table (deref (castPtr "info_table*" var_name))
          ] ++ ifSt conCase (alts >>= caseIf) [elseSt]
  where
    conCase = s "$$->type == 1" [info_table]    
    var_key = undefined -- get from the environment
    func_prefix = undefined -- get from the environment
    
    elseSt = [
       assert (s "$$ == $$" [ptrAccess info_table "type", "5"]),
       returnSt (funCall "thunk_continuation" [var_ref, name, "bindings", var_ref])
     ]
    caseIf (AltCase conName freeVars exp) = ifSt cond ifBody []
        where
          cond = assert (s "$$ == $$" [actualConNum, expectedConNum])
          ifBody = declInit (s "$$*" [conName]) conInnerName conCasted:{- Todo: need to bind the free vars of the case to the con-} eval exp
          conInnerName = undefined -- gen from env
          expectedConNum = undefined -- get from the environment
          conCasted = castPtr conName var_name
          actualConNum = structAccess (structAccess (ptrAccess info_table "extra") "constructor") "con_num"
    info_table = s "$$_info" [var_name]
    var_ref = s "$$_ref" [var_name]
    arg1 = "bindings"
    returnType = "ref"
    name = s "$$_$$" [func_prefix, "cont"]
    args = [("hash_map*", "bindings")]


{-
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref one_ref;
     get_binding(bindings, 0, &one_ref);
     push_ptr(one_ref);

     return plus_int_slow(one_ref);

-}    
generateThunkCont funName (THUNK e) = funcFormatter "ref" funName [("ref", "thunk_ref")] body
    where
      body = declInit "hash_map*" "bindings" "THUNK_GET_BINDINGS(thunk_ref)":eval e


    
eval :: Expression -> [String]
eval (Case _ _) = [returnSt (funCall case_con_name ["bindings"])]
    where
      case_con_name = undefined -- gen from env and record that a case needs to be built

-- (void**)get_binding()
eval (Primop "+#" [L (I x), L (I y)]) = eval (Atom . L . I $ x + y)
eval (FuncCall fun args)
     | knownAndSaturated fun args = [funCall (fast_call_name fun) (map extractArgsToFunArgs args)]
     | otherwise = pushFunArgs fun args ++ [decl "ref" tmp, funCall (slow_call_name fun) [tmp]]
     where
       tmp = undefined -- gen from env

{-
   let x' = THUNK (f x) in e

   ref thunk1_ref = create_thunk(bindings, map_thunk1);
   put_binding(bindings, 4, thunk1_ref);

-}
eval (Let var obj e) = evalObject thunk_name obj ++ [putBinding (show updateKey) thunk_ref_name] ++ eval e
    where
      thunk_name = var
      updateKey :: Int
      updateKey = undefined -- get from the environment (need map from var -> updKey)
      thunk_ref_name = s "$$_ref" [thunk_name]
eval (Atom (L (I x))) = [show x]
eval (Atom (V x)) = [x]

evalObject obj_name (THUNK e) = [declInit "ref" obj_name $ funCall "createThunk" ["bindings", thunk_cont]]
    where thunk_cont = undefined -- get from the environment, generate a new thunk cont name
{-
   Cons x' f'
//
   NEW_REF(cons_ref, Cons*, sizeof(Cons), cons)
   cons->info_ptr = &cons_info_table;
   cons->value = thunk1_ref;
   cons->next = thunk2_ref;
-}
evalObject obj_name (CON (Con c atoms)) = [newRefMacro ref_name (s "$$*" [c]) (s "sizeof($$)"[c]) val_name] ++ assignInfoPtr:assignFields
    where
      val_name = obj_name -- get from env
      c_info_table = undefined -- get from env
      fields = undefined -- get from env
      ref_name = s "$$_ref" [val_name]
      assignFields = map assignField (zip fields atoms)
      assignField (f, V x) = (ptrAccess val_name f) .= x
      assignField (f, L (I x)) = ptrAccess val_name f .= show x
      assignInfoPtr = ptrAccess val_name "info_ptr"  .= (s "&$$" [c_info_table])
{-
     let z = map_slow f
//
     NEW_REF(pap_ref, void**, sizeof(void*) + sizeof(ref), pap)

     NEW(struct info_table, pap_info)
     pap_info->type = 4;
     struct pap pap_info_extra = { .info_ptr = &plus_info_table, .size = 1 };
     pap_info->extra.pap_info = pap_info_extra;

     pap[0] = (void*)pap_info;
     *(ref*)(pap + 1) = one_ref;


-}
evalObject obj_name (PAP (Pap fun atoms)) = [newPap] ++ constructPapInfo ++ assignPapValues
    where
      pap_info_name = obj_name
      funInfoTable = undefined -- gen from the environment
      value_name = obj_name
      papSize = generatePapSize fun (length atoms)
      ref_name = s "$$_ref" [value_name]
      newPap = newRefMacro ref_name "void**" papSize value_name
      newPapInfo = newMacro "info_table" pap_info_name
      assignType = ptrAccess pap_info_name "type" .= show 4
      pap_info_extra_name = undefined -- s "$$_extra" pap_info_name
      initExtraInfo = declInit "struct pap" pap_info_extra_name (s " { .info_ptr = &$$, .size = $$" [funInfoTable, show (length atoms)])
      assignExtraInfo = structAccess (ptrAccess pap_info_name "extra") "pap_info" .= pap_info_extra_name
      constructPapInfo = [newPapInfo, assignType, initExtraInfo, assignExtraInfo]
      assignPapInfo = arrayIndex value_name 0 .= castPtr "void" pap_info_name
      assignPapValues = assignPapInfo:assignPapAtoms value_name fun atoms

