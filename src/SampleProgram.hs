import Types

{-
main = let one = I# 1
       let inc = THUNK (plus_int one)
       let inced = THUNK (map inc list)
       let list = Cons one inced
       let element1 = THUNK (head inced)
       let tail1 = THUNK (tail inced)
       let element2 = THUNK (head tail1)
       in case plus_int element1 element2 of
          I# z -> print_int z
-}

program :: Program
program = undefined -- [("main", THUNK expression), ("plus_int", FUNC func)]
    where
      {-func = Fun [("x", Boxed), ("y", Unboxed)]
                 (Case (Atom $ V "x") [AltCase "I#" ["z"] $
                              Case (Atom $ V "y")
                                   [AltCase "I#"  ["a"] $
                                       Let "b" (THUNK (Primop "+#" ["z", "a"])) $
                                           Let "c" (CON (Con "I#" ["b"])) (V "c")
                                           ]])
       -}
      expression = Let "one" one $
                   Let "inc" inc $
                   Let "list" list $
                   Let "element1" element1 $
                   Let "tail1" tail1 $
                   Let "element2" element2 $
                   Let "case_expr" (THUNK (FuncCall "plus_int" [V "element1", V "element2"])) $
                   Case (V "case_expr")
                        [AltCase "I#" ["x"] (Primop "print_int" [V "x"])]
      one = CON (Con "I#" [L (I 1)])
      inc = PAP (Pap "plus_int" [V "one"])
      inced = THUNK (FuncCall "map" [V "inc", V "list"])
      list = CON (Con "Cons" [V "one", V "inced"])
      element1 = THUNK (FuncCall "head" [V "inced"])
      tail1 = THUNK (FuncCall "tail" [V "inced"])
      element2 = THUNK (FuncCall "head" [V "tail1"])
                      
                 
                         
                       
