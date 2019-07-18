module SampleProgram where

import Types
import CodeGen
import Control.Monad
import Control.Monad.State    
import Control.Lens hiding (cons)
import Utils
import Main

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

pairConDecl = ConDecl "Pair" [pairCon]
    where
      pairCon = ConDefn "Pa" 0 [("P_fst", Boxed), ("P_snd", Boxed)]

listConDecl = ConDecl "List" [listCon, nilCon]
    where
      listCon = ConDefn "Cons" 0 [("C_element", Boxed), ("C_next", Boxed)]
      nilCon = ConDefn "Nil" 1 []

unitConDecl = ConDecl "Unit" [unitCon]
    where
      unitCon = ConDefn "Unit" 0 []


boolConDecl = ConDecl "Bool" [trueCon, falseCon]
    where
      trueCon = ConDefn "True" 0 []
      falseCon = ConDefn "False" 1 []

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
plus_int_test = Program [intConDecl, unitConDecl] [("plus_int", FUNC plus_int), ("main_", FUNC main_fun)]
    where
      main_fun = Fun [] e
          where e = Let "pl_x" (CON (Con "I" [L 2101])) $
                    Let "pl_y" (CON (Con "I" [L 2132])) $
                    Let "pl_z" (THUNK (FuncCall "plus_int" [V "pl_x", V "pl_y"])) $
                    Case (V "pl_z") [AltCase "I" ["pl_a"] $ Atom $ P $ Primop "#print_int" [V "pl_a"]]

{-
  plus_int x1 y1 = case x1 of
                    I# a1 -> case y1 of
                               I# b1 -> let c1 = I# (a1 #plus b1)
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
      primopSum = P $ Primop "#plus" [V "a1", V "b1"]



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
                    AltCase "Nil" [] (Atom . P $ Primop "#exception" [])
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
                    AltCase "Nil" [] (Atom . P $ Primop "#exception" [])
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
print_i_list :: List Int
print_i_list l = case l of
                 Cons v n -> case v of
                               I# x -> let rest = THUNK (print_i_list n) in
                                       let inner = THUNK (seq (print_int x) rest)
                                       seq inner ()

seq :: a -> b -> b
seq a b = case a of
            x -> b
-}

print_i_list_fn :: Function
print_i_list_fn = Fun [("pr_i_l", Boxed)] $ Case (V "pr_i_l") [
                     AltCase "Cons" ["pr_i_v", "pr_i_n"] $
                        Case (V "pr_i_v") $ [
                           AltCase "I" ["pr_i_x"] $
                              Let "pr_i_rest" (THUNK (FuncCall "print_i_list" [V "pr_i_n"])) $
                                  Let "pr_i_inner" (THUNK (FuncCall "seq" [P $ Primop "#print_int" [V "pr_i_x"], V "pr_i_rest"])) $
                                      Let "unit" (CON (Con "Unit" [])) $ 
                                          FuncCall "seq" [V "pr_i_inner", V "unit"]
                             
                                 ],
                     AltCase "Nil" [] $
                        Let "unit" (CON $ Con "Unit" []) $
                            Atom $ V "unit"
                  ]


seq_fn :: Function
seq_fn = Fun [("se_a", Boxed), ("se_b", Boxed)] $ Case (V "se_a") [AltForce "se_x" $ Atom (V "se_b")]
         
{-
  uncurry :: (a -> b -> c) -> (a, b) -> c
  uncurry f p = case p of
                  (a, b) -> f a b
-}
uncurry_fn :: Function
uncurry_fn = Fun [("un_f", Boxed), ("un_p", Boxed)] $
               Case (V "un_p") $ [
                         AltCase "Pa" ["un_a", "un_b"] $ FuncCall "un_f" [V "un_a", V "un_b"]
                         ]

{-
  index :: [a] -> Int# -> a
  index l x = case x #eq 0 of
                True -> head l
                False -> let n = tail l in 
                         index l n
                       
-}
index_fn :: Function
index_fn = Fun [("in_l", Boxed), ("in_x", Unboxed)] $
           Case (P $ Primop "#eq" [V "in_x", L 0]) [
                     AltCase "1" [] $ FuncCall "head" [V "in_l"],
                     AltCase "0" [] $ Let "in_n" (THUNK $ FuncCall "tail" [V "in_l"]) $
                                              FuncCall "index" [V "in_n", P $ Primop "#sub" [V "in_x", L 1]]
                     ]

{-
zip :: [a] -> [b] -> [(a, b)]
zip as bs = case as of
              Nil -> Nil
              Cons a an -> case bs of
                             Nil -> Nil
                             Cons b bn -> let p = CON (a, b) in
                                          let rest = THUNK (zip an bn) in
                                          let res = CON (Cons p rest) in
                                          res
-}
zip_fn :: Function
zip_fn = Fun [("zi_as", Boxed), ("zi_bs", Boxed)] $ Case (V "zi_as") [
             AltCase "Nil"[] $ Let "zi_nil" (CON $ Con "Nil" []) $ Atom (V "zi_nil"),
             AltCase "Cons" ["zi_a", "zi_an"] $ Case (V "zi_bs") [
                          AltCase "Nil" [] $ Let "zi_nil" (CON $ Con "Nil" []) $ Atom (V "zi_nil"),
                          AltCase "Cons" ["zi_b", "zi_bn"] $
                                  Let "zi_p" (CON pairedVal) $
                                      Let "zi_rest" (THUNK recursiveCall) $
                                          Let "zi_res" (CON $ Con "Cons" [V "zi_p", V "zi_rest"]) $
                                              Atom (V "zi_res")
                         ]
         ]
    where
      pairedVal = Con "Pa" [V "zi_a", V "zi_b"]
      recursiveCall = FuncCall "zip" [V "zi_an", V "zi_bn"]
                                           
         
{-
fibo_test = let zipped = THUNK (zip fibs fibTail) in
            let fibTail = THUNK (tail fibs) in
            let mapped = THUNK (map plus_uncurried zipped) in
            let zero = CON (I 0) in
            let one = CON (I 1) in
            let fibsT = CON (one : mapped) in
            let fibs = CON (zero : fibsT) in index fibs 20#
-}
fibo_test2 = Program [intConDecl, listConDecl, unitConDecl, pairConDecl, boolConDecl]
                    [("plus_int", FUNC plus_int),
                     ("head", FUNC head_fn),
                     ("tail", FUNC tail_fn),
                     ("map", FUNC map_fn),
                     ("zip", FUNC zip_fn),
                     ("uncurry", FUNC uncurry_fn),
                     ("seq", FUNC seq_fn),
                     ("index", FUNC index_fn),
                     ("main_", FUNC main_)]
    where
      main_ = Fun [] main_exp
      main_exp = Let "one" one $
                 Let "zero" zero $
                 Let "zipped" zipped $
                 Let "fibTail" fibTail $
                 Let "plus_uncurried" plus_uncurried $
                 Let "mapped" mapped $
                 Let "fibsT" fibsT $
                 Let "fibs" fibs $
                 Let "elem" elem $
                     Case (V "elem") [AltCase "I" ["x"] $ Atom $ P $ Primop "#print_int" [V "x"]]
                         where
                           elem = THUNK (FuncCall "index" [V "fibs", L 20])
                           one = CON (Con "I" [L 1])
                           zero = CON (Con "I" [L 0])
                           zipped = THUNK $ FuncCall "zip" [V "fibs", V "fibTail"]
                           fibTail = THUNK $ FuncCall "tail" [V "fibs"]
                           fibsT = CON $ Con "Cons" [V "one", V "mapped"]
                           fibs = CON $ Con "Cons" [V "zero", V "fibsT"]
                           plus_uncurried = PAP $ Pap "uncurry" [V "plus_int"]
                           mapped = THUNK $ FuncCall "map" [V "plus_uncurried", V "zipped"]


{-
fibo_test = let zipped = THUNK (zip fibs fibTail) in
            let fibTail = THUNK (tail fibs) in
            let mapped = THUNK (map plus_uncurried zipped) in
            let zero = CON (I 0) in
            let one = CON (I 1) in
            let fibsT = CON (one : mapped) in
            let fibs = CON (zero : fibsT) in print_i_list fibs
-}
fibo_test = Program [intConDecl, listConDecl, unitConDecl, pairConDecl]
                    [("plus_int", FUNC plus_int),
                     ("head", FUNC head_fn),
                     ("tail", FUNC tail_fn),
                     ("map", FUNC map_fn),
                     ("zip", FUNC zip_fn),
                     ("uncurry", FUNC uncurry_fn),
                     ("print_i_list", FUNC print_i_list_fn),
                     ("seq", FUNC seq_fn),
                     ("main_", FUNC main_)]
    where
      main_ = Fun [] main_exp
      main_exp = Let "one" one $
                 Let "zero" zero $
                 Let "zipped" zipped $
                 Let "fibTail" fibTail $
                 Let "plus_uncurried" plus_uncurried $
                 Let "mapped" mapped $
                 Let "fibsT" fibsT $
                 Let "fibs" fibs $ FuncCall "print_i_list" [V "fibs"]
                         where
                           one = CON (Con "I" [L 1])
                           zero = CON (Con "I" [L 0])
                           zipped = THUNK $ FuncCall "zip" [V "fibs", V "fibTail"]
                           fibTail = THUNK $ FuncCall "tail" [V "fibs"]
                           fibsT = CON $ Con "Cons" [V "one", V "mapped"]
                           fibs = CON $ Con "Cons" [V "zero", V "fibsT"]
                           plus_uncurried = PAP $ Pap "uncurry" [V "plus_int"]
                           mapped = THUNK $ FuncCall "map" [V "plus_uncurried", V "zipped"]

{-
list_test = let one = CON (I# 1) in
            let inc = PAP (plus_int 1) in
            let inced = THUNK (map inc list) in
            let list = CON (1 : inced) in
            let element1 = THUNK (head inced) in
            let tail1 = THUNK (tail inced) in
            let element2 = THUNK (head tail1) in
            let case_expr = THUNK (plus_int element1 element2) in
            case case_expr of
              I# x -> print_int x
This constructs an infinite list of increasing numbers `list` and sums the 2nd and 3rd elements
-}

list_test = Program [intConDecl, listConDecl, unitConDecl]
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
                        [AltCase "I" ["x"] (Atom $ P $ Primop "#print_int" [V "x"])]
      one = CON (Con "I" [L 1])
      inc = PAP (Pap "plus_int" [V "one"])
      inced = THUNK (FuncCall "map" [V "inc", V "list"])
      list = CON (Con "Cons" [V "one", V "inced"])
      element1 = THUNK (FuncCall "head" [V "inced"])
      tail1 = THUNK (FuncCall "tail" [V "inced"])
      element2 = THUNK (FuncCall "head" [V "tail1"])




list_test2 = Program [intConDecl, listConDecl, unitConDecl]
                    [("plus_int", FUNC plus_int),
                     ("head", FUNC head_fn),
                     ("tail", FUNC tail_fn),
                     ("map", FUNC map_fn),
                     ("print_i_list", FUNC print_i_list_fn),
                     ("seq", FUNC seq_fn),
                     ("main_", FUNC main_)]
    where
      main_ = Fun [] main_exp
      main_exp = Let "one" one $
              Let "inc" inc $
              Let "inced" inced $
              Let "list" list $
              FuncCall "print_i_list" [V "list"]
      one = CON (Con "I" [L 1])
      inc = PAP (Pap "plus_int" [V "one"])
      inced = THUNK (FuncCall "map" [V "inc", V "list"])
      list = CON (Con "Cons" [V "one", V "inced"])

testSuite = [(plus_int_test, "plus_int_test"),
             (list_test, "list_test"),
             (list_test2, "list_test2"),
             (fibo_test, "fibo_test"),
             (fibo_test2, "fibo_test2"),
             (brokenTest2, "broken_test2")]            


test = do
  mapM (uncurry compile) testSuite



anotherTest = Program [
    ConDecl "Int" [ConDefn {conName = "I'", conTag = 0, conFields = [("I_val",Unboxed)]}],
    ConDecl "Pair" [ConDefn {conName = "Pa", conTag = 0, conFields = [("P_fst",Unboxed),("P_snd",Unboxed)]}],
    ConDecl "List" [ConDefn {conName = "Cons", conTag = 0, conFields = [("C_element",Unboxed),("C_next",Unboxed)]},
                    ConDefn {conName = "Nil", conTag = 1, conFields = []}],
    ConDecl "Unit" [ConDefn {conName = "Unit", conTag = 0, conFields = []}],
    ConDecl "Bool" [ConDefn {conName = "True", conTag = 0, conFields = []},ConDefn {conName = "False", conTag = 1, conFields = []}]] 
  
  [
    ("plus_int", FUNC (Fun [("x1",Boxed),("y1",Boxed)] (Case (V "x1") [
        AltCase "I'" ["a1"] (Case (V "y1") [
          AltCase "I'" ["b1"] (Let "c1" (CON (Con "I'" [P (Primop "#plus" [V "a1",V "b1"])]))
            (Atom (V "c1")))])]))),
    ("main_", THUNK (Let "one" (CON (Con "I'" [L 1])) 
                      (Let "inc" (THUNK (FuncCall "plus_int" [V "one"])) 
                        (Let "inced" (THUNK (FuncCall "map" [V "inc",V "list"]))
                          (Let "list" (CON (Con "Cons" [V "one",V "inced"])) 
                            (Let "element1" (THUNK (FuncCall "head" [V "inced"])) 
                              (Let "tail1" (THUNK (FuncCall "tail" [V "inced"])) 
                                (Let "element2" (THUNK (FuncCall "head" [V "tail1"])) 
                                  (Let "element3" (THUNK (FuncCall "plus_int" [V "element1",V "element2"])) 
                                    (Case (V "element3") [
                                        AltCase "I'" ["z"] (Atom $ P $ Primop "#print_int" [V "z"])])))))))))),
    ("map", FUNC (Fun [("f",Boxed),("l",Boxed)] (Case (V "l") [
        AltCase "Cons" ["v","n"] (Let "new_v" (THUNK (FuncCall "f" [V "v"])) 
                                    (Let "new_n" (THUNK (FuncCall "map" [V "f",V "n"])) 
                                      (Let "new_res" (CON (Con "Cons" [V "new_v",V "new_n"])) 
                                        (Atom (V "new_res"))))),
        AltCase "Nil" [] (Let "res" (CON (Con "Nil" [])) 
                            (Atom (V "res")))]))),
    ("head",FUNC (Fun [("l",Boxed)] (Case (V "l") [
      AltCase "Cons" ["v","n"] (Atom (V "v")),
      AltCase "Nil" [] (Atom . P $ Primop "#exception" [])]))),
    ("tail", FUNC (Fun [("l",Boxed)] (Case (V "l") [
      AltCase "Cons" ["v","n"] (Atom (V "v")),
      AltCase "Nil" [] (Atom . P $ Primop "#exception" [])])))
  ]


brokenTest2 = Program 
  [
    ConDecl "Int" [ConDefn {conName = "I", conTag = 0, conFields = [("I_valB",Unboxed)]}],
    ConDecl "Pair" [ConDefn {conName = "Pa", conTag = 0, conFields = [("P_fst",Boxed),("P_snd",Boxed)]}],
    ConDecl "List" [ConDefn {conName = "Cons", conTag = 0, conFields = [("C_element",Boxed),("C_next",Boxed)]},
    ConDefn {conName = "Nil", conTag = 1, conFields = []}],
    ConDecl "Unit" [ConDefn {conName = "Unit", conTag = 0, conFields = []}],
    ConDecl "Bool" [ConDefn {conName = "True", conTag = 0, conFields = []},ConDefn {conName = "False", conTag = 1, conFields = []}]
  ] 
  [
    ("plus_int",
      FUNC (Fun [("plus_int_x1",Boxed),("plus_int_y1",Boxed)] 
        (Case (V "plus_int_x1") [
          AltCase "I" ["plus_int_a1"] (Case (V "plus_int_y1") 
            [AltCase "I" ["plus_int_b1"] 
              (Let "plus_int_c1" (CON (Con "I" [P (Primop "#plus" [V "plus_int_a1",V "plus_int_b1"])]))
                (Atom (V "plus_int_c1")))])]))),
    ("head",FUNC (Fun [("head_l",Boxed)] (Case (V "head_l") [
        AltCase "Cons" ["head_v","head_n"] (Atom (V "head_v")),
        AltCase "Nil" [] (Atom (P (Primop "#exception" [])))]))),
    ("tail",FUNC (Fun [("tail_l",Boxed)] (Case (V "tail_l") [
      AltCase "Cons" ["tail_v","tail_n"] (Atom (V "tail_n")),
      AltCase "Nil" [] (Atom (P (Primop "#exception" [])))]))),
    ("map", FUNC (Fun [("map_f",Boxed),("map_l",Boxed)] 
                  (Case (V "map_l") [
                      AltCase "Cons" ["map_v","map_n"] 
                        (Let "map_new_v" (THUNK (FuncCall "map_f" [V "map_v"])) 
                          (Let "map_new_n" (THUNK (FuncCall "map" [V "map_f",V "map_n"])) 
                            (Let "map_new_res" (CON (Con "Cons" [V "map_new_v",V "map_new_n"])) 
                              (Atom (V "map_new_res"))))),
                      AltCase "Nil" [] (Let "map_res" (CON (Con "Nil" [])) (Atom (V "map_res")))]))),
    ("main_",FUNC (Fun [] 
                (Let "main__one" (CON (Con "I" [L 1])) 
                  (Let "main__inc" (PAP (Pap "plus_int" [V "main__one"])) 
                    (Let "main__inced" (THUNK (FuncCall "map" [V "main__inc",V "main__list"])) 
                      (Let "main__list" (CON (Con "Cons" [V "main__one",V "main__inced"])) 
                        (Let "main__element1" (THUNK (FuncCall "head" [V "main__inced"])) 
                          (Let "main__tail1" (THUNK (FuncCall "tail" [V "main__inced"])) 
                            (Let "main__element2" (THUNK (FuncCall "head" [V "main__tail1"])) 
                              (Let "main__element3" (THUNK (FuncCall "plus_int" [V "main__element1",V "main__element2"])) 
                                (Case (V "main__element3") [
                                    AltCase "I" ["main__z"] (Atom (P (Primop "#print_int" [V "main__z"])))])))))))))))]


brokenTest = Program 
                [
                  ConDecl "Int" [ConDefn {conName = "I", conTag = 0, conFields = [("I_valB",Unboxed)]}],
                  ConDecl "Pair" [ConDefn {conName = "Pa", conTag = 0, conFields = [("P_fst",Boxed),("P_snd",Boxed)]}],
                  ConDecl "List" [ConDefn {conName = "Cons", conTag = 0, conFields = [("C_element",Boxed),("C_next",Boxed)]},
                  ConDefn {conName = "Nil", conTag = 1, conFields = []}],
                  ConDecl "Unit" [ConDefn {conName = "Unit", conTag = 0, conFields = []}],
                  ConDecl "Bool" [ConDefn {conName = "True", conTag = 0, conFields = []},ConDefn {conName = "False", conTag = 1, conFields = []}]
                ] 
                [
                  ("plus_int",
                    FUNC (Fun [("plus_int_x1",Boxed),("plus_int_y1",Boxed)] 
                      (Case (V "plus_int_x1") [
                        AltCase "I" ["plus_int_a1"] (Case (V "plus_int_y1") 
                          [AltCase "I" ["plus_int_b1"] 
                            (Let "plus_int_c1" (CON (Con "I" [P (Primop "#plus" [V "plus_int_a1",V "plus_int_b1"])]))
                              (Atom (V "plus_int_c1")))])]))),
                  ("map", FUNC (Fun [("map_f",Boxed),("map_l",Boxed)] 
                                  (Case (V "map_l") [
                                    AltCase "Cons" ["map_v","map_n"] 
                                      (Let "map_new_v" (THUNK (FuncCall "map_f" [V "map_v"])) 
                                        (Let "map_new_n" (THUNK (FuncCall "map" [V "map_f",V "map_n"])) 
                                          (Let "map_new_res" (CON (Con "Cons" [V "map_new_v",V "map_new_n"])) 
                                            (Atom (V "map_new_res"))))),
                                    AltCase "Nil" [] (Let "map_res" (CON (Con "Nil" [])) (Atom (V "map_res")))]))),
                  ("head",FUNC (Fun [("head_l",Boxed)] (Case (V "head_l") [
                      AltCase "Cons" ["head_v","head_n"] (Atom (V "head_v")),
                      AltCase "Nil" [] (Atom (P (Primop "#exception" [])))]))),
                  ("tail",FUNC (Fun [("tail_l",Boxed)] (Case (V "tail_l") [
                    AltCase "Cons" ["tail_v","tail_n"] (Atom (V "tail_v")),
                    AltCase "Nil" [] (Atom (P (Primop "#exception" [])))]))),
                  ("main_",FUNC (Fun [] 
                              (Let "main__one" (CON (Con "I" [L 1])) 
                                (Let "main__inc" (PAP (Pap "plus_int" [V "main__one"])) 
                                  (Let "main__inced" (THUNK (FuncCall "map" [V "main__inc",V "main__list"])) 
                                    (Let "main__list" (CON (Con "Cons" [V "main__one",V "main__inced"])) 
                                      (Let "main__element1" (THUNK (FuncCall "head" [V "main__inced"])) 
                                        (Let "main__tail1" (THUNK (FuncCall "tail" [V "main__inced"])) 
                                          (Let "main__element2" (THUNK (FuncCall "head" [V "main__tail1"])) 
                                            (Let "main__element3" (THUNK (FuncCall "plus_int" [V "main__element1",V "main__element2"])) 
                                              (Case (V "main__element3") [
                                                  AltCase "I" ["main__z"] (Atom (P (Primop "#print_int" [V "main__z"])))])))))))))))]
              