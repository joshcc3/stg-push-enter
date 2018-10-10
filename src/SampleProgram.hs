module SampleProgram where

import Types
import CodeGen
import Control.Monad
import Control.Monad.State    
import Control.Lens hiding (cons)
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

listConDecl = ConDecl "List" [listCon, nilCon]
    where
      listCon = ConDefn "Cons" 0 [("C_element", Boxed), ("C_next", Boxed)]
      nilCon = ConDefn "Nil" 1 []


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
-}
plus_int_test = Program [intConDecl] [("plus_int", FUNC plus_int), ("main_", FUNC main_fun)]
    where
      main_fun = Fun [] e
          where e = Let "pl_x" (CON (Con "I" [L 2101])) $
                    Let "pl_y" (CON (Con "I" [L 2132])) $
                    Let "pl_z" (THUNK (FuncCall "plus_int" [V "pl_x", V "pl_y"])) $
                    Case (V "pl_z") [AltCase "I" ["pl_a"] $ Atom $ P $ Primop "print_int" [V "pl_a"]]

{-
  plus_int x1 y1 = case x1 of
                    I# a1 -> case y1 of
                               I# b1 -> let c1 = I# (a1 +# b1) 
                                        in c1
-}
plus_int = Fun [("x1", Boxed), ("y1", Boxed)] e
    where
      e = Case (V "x1") [AltCase "I" ["a1"] $
                         Case (V "y1") [AltCase "I" ["b1"] $
                                        Let "c1" (CON $ Con "I" [primopSum]) $
                                            Atom (V "c1")
                                       ]
                        ]
      primopSum = P $ Primop "+#" [V "a1", V "b1"]



{-
head :: [a] -> a
head l = case l of
           Cons v n -> v
           Nil -> exception
-}
head_fn :: Function
head_fn = Fun [("he_l", Boxed)] $
          Case (V "he_l") $ [
                    AltCase "Cons" ["he_v", "he_n"] (Atom $ V "he_v"),
                    AltCase "Nil" [] (Atom . P $ Primop "exception" [])
                    ]


{-
tail :: [a] -> a
tail l = case l of
           Cons v n -> v
           Nil -> exception
-}
tail_fn :: Function
tail_fn = Fun [("ta_l", Boxed)] $
          Case (V "ta_l") $ [
                    AltCase "Cons" ["ta_v", "ta_n"] (Atom $ V "ta_n"),
                    AltCase "Nil" [] (Atom . P $ Primop "exception" [])
                    ]


nil = Con "Nil" []
cons x v = Con "Cons" [x, v]      

{-
map :: (a -> b) -> [a] -> [b]
map f l = case l of
            Cons v n -> let new_v = THUNK (f v) in
                        let new_n = THUNK (map f n) in
                        let new_res = Cons new_v new_n in
                        new_res
            Nil -> Nil
-}
map_fn :: Function
map_fn = Fun args body
    where
      args = [("ma_f", Boxed), ("ma_l", Boxed)]
      body = Case (V "ma_l") [
              AltCase "Cons" ["ma_v", "ma_n"] $
                        Let "ma_new_v" (THUNK (FuncCall "ma_f" [V "ma_v"])) $
                            Let "ma_new_n" (THUNK (FuncCall "map" [V "ma_f", V "ma_n"]))$
                                Let "ma_new_res" (CON (cons (V "ma_new_v") (V "ma_new_n"))) $
                                  Atom (V "ma_new_res"),
                                                   
                AltCase "Nil" [] (Let "ma_z" (CON nil) (Atom $ V "ma_z"))
              ]


{-

-}
list_test = Program [intConDecl, listConDecl]
                    [("plus_int", FUNC plus_int), ("head", FUNC head_fn), ("tail", FUNC tail_fn), ("map", FUNC map_fn), ("main_", FUNC main_)]
    where
      main_ = Fun [] main_exp
      main_exp = Let "one" one $
              Let "inc" inc $
              Let "inced" inced $
              Let "list" list $
              Let "element1" element1 $
              Let "tail1" tail1 $
              Let "element2" element2 $
              Let "case_expr" (THUNK (FuncCall "plus_int" [V "element1", V "element2"])) $
              Case (V "case_expr")
                        [AltCase "I" ["x"] (Atom $ P $ Primop "print_int" [V "x"])]
      one = CON (Con "I" [L 1])
      inc = PAP (Pap "plus_int" [V "one"])
      inced = THUNK (FuncCall "map" [V "inc", V "list"])
      list = CON (Con "Cons" [V "one", V "inced"])
      element1 = THUNK (FuncCall "head" [V "inced"])
      tail1 = THUNK (FuncCall "tail" [V "inced"])
      element2 = THUNK (FuncCall "head" [V "tail1"])


