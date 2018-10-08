module SampleProgram where

import Types
import CodeGen
import Control.Monad
import Control.Monad.State    
import Control.Lens
import Utils

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
intConDecl = ConDecl "Int" [intConstructor]
    where
      intConstructor = ConDefn "I" 0 [("I_value", Unboxed)]      


test1 = [("main_function", FUNC fun)]
    where
      fun = Fun [("x", Boxed)] (Atom $ V "x")

{-
  data Int = I# Int

  main = let x = I# 1 in
         let y = I# 2 in
         let z = plus_int x y in
         case z of
           I# a -> print a

  plus_int x1 y1 = case x1 of
                    I# a1 -> case y1 of
                               I# b1 -> let c1 = THUNK (a1 +# b1) in c1
         
  
-}
plus_int_test = Program [intConDecl] [("plus_int", FUNC plus_int), ("main_", FUNC main_fun)]
    where
      main_fun = Fun [] e
          where e = Let "x" (CON (Con "I" [L 1])) $
                    Let "y" (CON (Con "I" [L 2])) $
                    Let "z" (THUNK (FuncCall "plus_int" [V "x", V "y"])) $
                    Case (V "z") [AltCase "I" ["a"] $ Primop "print_int" [V "a"]]
      plus_int = Fun [("x1", Boxed), ("y1", Boxed)] e
          where e = Case (V "x1") [AltCase "I" ["a1"] $
                        Case (V "y1") [AltCase "I" ["b1"] $
                            Let "c1" (THUNK $ Primop "+#" [V "a1", V "b1"]) $
                               Atom (V "c1")]]


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
      one = CON (Con "I#" [L 1])
      inc = PAP (Pap "plus_int" [V "one"])
      inced = THUNK (FuncCall "map" [V "inc", V "list"])
      list = CON (Con "Cons" [V "one", V "inced"])
      element1 = THUNK (FuncCall "head" [V "inced"])
      tail1 = THUNK (FuncCall "tail" [V "inced"])
      element2 = THUNK (FuncCall "head" [V "tail1"])

