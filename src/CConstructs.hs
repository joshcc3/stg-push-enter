module CConstructs where

import Utils
import Types    

newMacro typ nm = s "NEW($$, $$)" [typ, nm]
initBindings = [decl "hash_map *" "bindings", funCall "init_bindings" [reference "bindings"]]
putBinding thunk_ref_name updateKey = funCall "putBinding" ["bindings", show updateKey, thunk_ref_name]

funInfoTableName name = s "$$_info_table" [name]

tab = map ('\t':)

typedef name fields = s "typedef struct $$ {" [name]: tab (map fieldDecl fields) ++ [s "} $$;" [name]]
  where
    fieldDecl (f, Unboxed) = s "ref $$;" [f]
    fieldDecl (f, Boxed) = s "int $$;" [f]

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
declInit typ name val = s "$$ $$ = $$;" [typ, name, val]

funcFormatter returnType name args body = [line1, "{"] ++ body ++ ["}"]
    where
      line1 = s "$$ $$($$)" [returnType, name, commaSep . map (\(a, b) -> s "$$ $$" [a, b]) $ args]

newRefMacro refn typ size valn = s "NEW_REF($$, $$, $$, $$)" [refn, typ, size, valn]
bindingMacro ref typ val updk bs = s "GET_BINDING($$, $$, $$, $$, $$)" [ref, typ, val, updk, bs]
ptrAccess v f = s "($$)->$$" [v, f]
cast t n = s "($$)$$" [t, n]
castPtr t n = s "($$*)$$" [t, n]
returnSt st = s "return $$;" [st]
assert st = s "assert($$);" [st]
structAccess var field = s "($$).$$" [var, field]
deref x = s "*($$)" [x]
reference x = s "&($$)" [x]
a ..= b = s "$$ = $$;" [a, b]
arrayIndex v i = s "$$[$$]" [v, show i]

funCall name args = s "$$($$)" [name, commaSep args]

commaSep = charSeperate ','

charSeperate c [] = ""
charSeperate c xs = s "$$ $$" [init xs >>= \x -> s "$$$$ " [x, [c]], last xs]



extractArgsToFunArgs (V x) = x
extractArgsToFunArgs (L (I x)) = show x
fast_call_name f = s "$$_fast" [f]
slow_call_name f = s "$$_slow" [f]

bracketInit typ as = cast typ $ s "{$$}" [commaSep (map assign as)]
    where
      assign (field, val) = s ".$$ = $$" [field, val]

toSize Boxed = "sizeof(ref)"
toSize Unboxed = "sizeof(int)"

