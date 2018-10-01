{-# LANGUAGE LambdaCase #-}

import Types
import Utils

tab = map ('\t':)

ifSt cond ifBody elses = [condSt, "{"] ++ tab ifBody ++ ["}"] ++ (
                         case elses of
                           [] -> []
                           es -> let elseSt = ["else", "{"] ++ tab (last es) ++ ["}"] in 
                                 let elseifs = init es >>= toElseIf
                                 in elseifs ++ elseSt
                                                               )
    where
      toElseIf a = s "else $$" [head a] : tab (tail a)
      condSt = s "if($$)" [cond]

decl typ name = s "$$ $$;" [typ, name]
generatePapSize = undefined -- generate the pap size from the atoms
declInit typ name val = s "$$ $$ = $$;" [typ, name, val]
funcFormatter returnType name args body = [line1, "{"] ++ body ++ ["}"]
    where
      line1 = s "$$ $$($$)" [returnType, name, commaSep . map (\(a, b) -> s "$$ $$" [a, b]) $ args]

newRefMacro refn typ size valn = s "NEW_REF($$, $$, $$, $$)" [refn, typ, size, valn]
bindingMacro ref typ val updk bs = s "GET_BINDING($$, $$, $$, $$, $$)" [ref, typ, val, updk, bs]
ptrAccess v f = s "($$)->$$" [v, f]
castPtr t n = s "($$*)$$" [t, n]
returnSt st = s "return $$;" [st]
assert st = s "assert($$);" [st]
structAccess var field = s "($$).$$" [var, field]
deref x = s "*($$)" [x]
a .= b = s "$$ = $$;" [a, b]
arrayIndex v i = s "$$[$$]" [v, show i]

funCall name args = s "$$($$)" [name, commaSep args]

commaSep [] = ""
commaSep xs = s "$$ $$" [init xs >>= \x -> s "$$, " [x], last xs]

assignPapAtoms pap fun atoms = undefined -- gen from env

extractArgsToFunArgs (V x) = x
extractArgsToFunArgs (L (I x)) = show x
toPrimOpArgs = undefined -- gen from env
knownAndSaturated f as = undefined -- gen from env
fast_call_name f = s "$$_fast" [f]
slow_call_name f = s "$$_slow" [f]
pushFunArgs = undefined -- gen from env

{-
Notes:
Env:
Binding key of free variables

Mapping from var to it's updatekey
queue of (thunk expressions, thunk names) to generate functions for

Mapping from the constructor name to its info table in Haskell 
  info table must have all the fields
Mapping from the constructor name to its accessor names
-}


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
generateCaseCont (Case (V var_name) [AltCase conName freeVars exp]) = funcFormatter returnType name args body
  where
    var_key = undefined -- get from the environment
    func_prefix = undefined -- get from the environment
    conCasted = castPtr conName var_name
    ifBody = assert (s "$$ == $$" [actualConNum, expectedConNum]):declInit (s "$$*" [conName]) conInnerName conCasted:{- Todo: need to bind the free vars of the case to the con-} eval exp
       where
         conInnerName = undefined -- gen from env
         fieldName = undefined -- get from the environment
         actualConNum = structAccess (structAccess (ptrAccess info_table "extra") "constructor") "con_num"
         expectedConNum = undefined -- get from the environment
    elseSt = [
       assert (s "$$ == $$" [ptrAccess info_table "type", "5"]),
       returnSt (funCall "thunk_continuation" [var_ref, name, "bindings", var_ref])
     ]
    
    info_table = s "$$_info" [var_name]
    var_ref = s "$$_ref" [var_name]
    arg1 = "bindings"
    returnType = "ref"
    name = s "$$_$$" [func_prefix, "cont"]
    args = [("hash_map*", "bindings")]
    cond = s "$$->type == 1" [info_table]
    body =[
         bindingMacro var_ref "void**" var_name var_key arg1,
         declInit "info_table*" info_table (deref (castPtr "info_table*" var_name))
        ] ++  ifSt cond ifBody [elseSt]


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
eval (Primop fun atoms) = [funCall fun (toPrimOpArgs fun atoms)]
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
eval (Let var obj e) = evalObject thunk_name obj ++ [putBinding] ++ eval e
    where
      thunk_name = s "thunk_$$" [undefined] -- get suffix from the environment
      thunk_ref_name = s "$$_ref" [thunk_name]
      updateKey = undefined -- get from the environment (need map from var -> updKey)
      putBinding = funCall "putBinding" ["bindings", updateKey, thunk_ref_name]
eval (Atom (L (I x))) = [show x]
eval (Atom (V x)) = [x]

evalObject obj_name (THUNK e) = [declInit "ref" obj_name $ funCall "createThunk" ["bindings", thunk_cont]] where thunk_cont = undefined -- get from the environment
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
      value_name = obj_name
      papSize = generatePapSize fun (length atoms)
      ref_name = s "$$_ref" [value_name]
      newPap = newRefMacro ref_name "void**" papSize value_name
      pap_info_name = undefined -- gen from the environment
      funInfoTable = undefined -- gen from the environment
      newPapInfo = newMacro "info_table" pap_info_name
      assignType = ptrAccess pap_info_name "type" .= show 4
      pap_info_extra_name = s "$$_extra" pap_info_name
      initExtraInfo = declInit "struct pap" pap_info_extra_name (s " { .info_ptr = &$$, .size = $$" [funInfoTable, show (length atoms)])
      assignExtraInfo = structAccess (ptrAccess pap_info_name "extra") "pap_info" .= pap_info_extra_name
      constructPapInfo = [newPapInfo, assignType, initExtraInfo, assignExtraInfo]
      assignPapInfo = arrayIndex value_name 0 .= castPtr "void" pap_info_name
      assignPapValues = assignPapInfo:assignPapAtoms value_name fun atoms

newMacro typ nm = s "NEW($$, $$)" [typ, nm]
