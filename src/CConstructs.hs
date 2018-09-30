module CConstructs where

import Utils
import Types    

main_entry_point = "main_"

st x = s "$$;" [x]

to_temp_var i = s "var_$$" [show i]

newMacro typ nm = s "NEW($$, $$)" [typ, nm]
initBindings = [decl "hash_map *" "bindings", st $ st $ funCall "init_bindings" [reference "bindings"]]
putBinding :: String -> Int -> String
putBinding thunk_ref_name updateKey = st $ funCall "put_binding" ["bindings", show updateKey, thunk_ref_name]

funInfoTableName name = s "$$_info_table" [name]

tab = map ('\t':)

typedef name fields internalFields = s "typedef struct $$ {" [name]: tab (map internalFieldDecl internalFields ++ map fieldDecl fields) ++ [s "} $$;" [name]]
  where
    internalFieldDecl (t, n) = s "$$ $$;" [t, n]
    fieldDecl (f, Unboxed) = s "int $$;" [f]
    fieldDecl (f, Boxed) = s "ref $$;" [f]

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

funcFormatter returnType name args body = [line1, "{"] ++ tab body ++ ["}"]
    where
      line1 = s "$$ $$($$)" [returnType, name, commaSep . map (\(a, b) -> s "$$ $$" [a, b]) $ args]

newRefMacro refn typ size valn = s "NEW_REF($$, $$, $$, $$)" [refn, typ, size, valn]
bindingMacro ref typ val updk bs = s "GET_BINDING($$, $$, $$, $$, $$)" [ref, typ, val, updk, bs]

getBinding :: Int -> String -> String
getBinding updateKey refName = st $ funCall "get_binding" ["bindings", show updateKey, reference refName]

ptrAccess v f = s "($$)->$$" [v, f]
cast t n = s "($$)$$" [t, n]
castPtr t n = s "($$*)$$" [t, n]
returnSt st = s "return $$;" [st]
assert st = s "assert($$);" [st]
structAccess var field = s "($$).$$" [var, field]
deref x = s "*($$)" [x]
reference x = s "&($$)" [x]
a ..= b = s "$$ = $$;" [a, b]
arrayIndex :: String -> Int -> String
arrayIndex v i = s "$$[$$]" [v, show i]

funCall name args = s "$$($$)" [name, commaSep args]

commaSep = charSeperate ','

charSeperate c [] = ""
charSeperate c [x] = x
charSeperate c xs = s "$$ $$" [init xs >>= \x -> s "$$$$ " [x, [c]], last xs]



extractArgsToFunArgs (V x) = x
extractArgsToFunArgs (L x) = show x
fast_call_name f = f -- s "$$_fast" [f]
slow_call_name f = s "$$_slow" [f]

bracketInit typ as = cast typ $ s "{$$}" [commaSep (map assign as)]
    where
      assign (field, val) = s ".$$ = $$" [field, val]

toSize Boxed = "sizeof(ref)"
toSize Unboxed = "sizeof(int)"


c_sum = charSeperate '+'

declare_var_type (V x, Boxed) = decl "ref" x
declare_var_type (V x, Unboxed) = decl "int" x
pop_instr (V x, Boxed) = st $ funCall "pop_ptr" . (:[]) . reference $ x
pop_instr (V x, Unboxed) = st $ funCall "pop_int" . (:[]) . reference $ x

push_instr (V x, Boxed) = st $ funCall "push_ptr" . (:[]) $ x
push_instr (V x, Unboxed) = st $ funCall "push_int" . (:[]) $ x
push_instr (L x, Unboxed) = st $ funCall "push_int" . (:[]) $ show x
        
includeSys x = s "#include <$$>" [x]
includeUser x = s "#include $$" [show x]

newScope :: [Statement] -> [Statement]
newScope st = "{":tab st ++ ["}"]
