module CConstructs where

import Control.Monad
import Control.Lens
import qualified Data.Map as M
import Utils
import Types    

main_entry_point = "main_"

st x = s "$$;" [x]

to_temp_var i = s "var_$$" [show i]

newMacro typ nm = s "NEW($$, $$)" [typ, nm]


initBindings = do
  res <- decl "hash_map *" "bindings"
  return [res, st $ st $ funCall "init_bindings" [reference "bindings"]]
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

decl :: String -> String -> MonStack String
decl typ name = do
  liveVars %= M.insert name typ
  return $ s "$$ $$;" [typ, name]
         
declInit :: String -> String -> String -> MonStack String
declInit typ name val = do
    liveVars %= M.insert name typ
    return $ s "$$ $$ = $$;" [typ, name, val]

funcFormatter returnType name args body = [line1, "{"] ++ tab body ++ ["}"]
    where
      line1 = s "$$ $$($$)" [returnType, name, commaSep . map (\(a, b) -> s "$$ $$" [a, b]) $ args]

newRefMacro :: String -> String -> String -> String -> MonStack String
newRefMacro refn typ size valn = do
    liveVars %= M.insert refn "ref" . M.insert valn typ
    return $ s "NEW_REF($$, $$, $$, $$)" [refn, typ, size, valn]


bindingMacro :: String -> String -> String -> String -> String -> MonStack String
bindingMacro ref typ val updk bs = do
   liveVars %= M.insert ref "ref" . M.insert val typ
   return $ s "GET_BINDING($$, $$, $$, $$, $$)" [ref, typ, val, updk, bs]

getBinding :: Int -> String -> String
getBinding updateKey refName = st $ funCall "get_binding" ["bindings", show updateKey, reference refName]

ptrAccess v f = s "($$)->$$" [v, f]
cast t n = s "($$)$$" [t, n]
castPtr t n = s "($$*)$$" [t, n]
returnSt st = s "return $$;" [st]

-- rdx, rcx, rbx              
tailCall f args = if length args > 6 then error "Err" else
    "__asm__ volatile (":
    (tab $
     concat (zipWith argPushes [0..] args) ++
     "\"movq %%rbp, %%rsp;\\n\\t\"":
     "\"popq %%rbp;\\n\\t\"":
     [":", asmInputOperands]) ++ [");"] ++
    [s "goto *(void*)$$;" [f]]
        where
          asmInputOperands = s ":$$" [commaSep $ map g args]
              where g (x, _) = s "\"r\"($$)" [x]
          argPushes i (x, Unboxed) = [s "\"xorq %%$$, %%$$\\n\\t\"" [reg 'r' i, reg 'r' i],
                                      s "\"movl %$$, %%$$;\\n\\t\"" [show i, reg 'e' i]]
          argPushes i (x, Boxed) = [s "\"movq %$$, %%$$;\\n\\t\"" [show i, reg 'r' i]]
          reg 'e' i | i < 4 = map ('e':) mainRegs !! i -- r8, r9, r11, then stack
                    | otherwise = map (++"d") suppRegs !! (i - 4)
          reg 'r' i | i < 4 = map ('r':) mainRegs !! i -- r8, r9, r11, then stack
                    | otherwise = suppRegs !! (i - 4)
          mainRegs = ["di", "si", "dx", "cx"]
          suppRegs = ["r8", "r9"]

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



extractArgsToFunArgs (V x, Nothing) = x
extractArgsToFunArgs (L x, Nothing) = show x
extractArgsToFunArgs (P _, Just a) = a
fast_call_name f = f
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

cOps :: M.Map String String
cOps = M.fromList  [("+#", "+"), ("-#", "-"), ("==#", "=="), (">#", ">"), ("<#", "<")]

binPrimops ::  M.Map String (Int -> Int -> Int)
binPrimops = M.fromList [("+#", (+)), ("-#", (-)), ("==#", eq), (">#", gt), ("<#", lt)]
    where
      eq x y | x == y = 1
             | otherwise = 0
      gt x y | x > y = 1
             | otherwise = 0
      lt x y | x < y = 1
             | otherwise = 0
