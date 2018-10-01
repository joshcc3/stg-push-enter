import Utils

{-
Notes:
Env:
Binding key of free variables

Mapping from the constructor name to its info table in Haskell
Mapping from the constructor name to its accessor names
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
ifSt = undefined


eval :: Expression -> String
eval (Case (V varName) [AltCase conName freeVars exp]) = func_formatter returnType name args body
  where
	l_key = undefined -- get from the environment
	prefix = undefined -- get from the environment
	ifBody = undefined -- TODO need to write
	elseSt = undefined -- TODO Need to write

    info_table = s "$$_info" [varName]
    var_ref = s "$$_ref" [varName]
    arg1 = "bindings"
    returnType = "ref"
    name = s "$$_$$" [prefix, "cont"]
	args = [("hash_map*", "bindings")]
	cond = s "$$->type == $$" [info_table]
	body =[
		bindingMacro var_ref "void**" var_name l_key arg1,
		decl_case "info_table*" info_table "*(info_table**)", varName],
		ifSt cond ifBody [elseSt]
	]



