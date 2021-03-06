{-# LANGUAGE ExistentialQuantification #-}


module ParserTest where

import Parser
import Text.Parsec
import Types
import Text.Parsec.Token
import Control.Monad

t1 = ("1", lit, L 1)

t2 = ("case x of\n  Pair a b -> #plus a b;",
      caseExpr,
      Case (V "x") [AltCase "Pair" ["a","b"] (Atom (P (Primop "#plus" [V "a",V "b"])))]
      )

t3 = ("let one = I 1 in one", letExpr, Let "one" (CON (Con "I" [L 1])) (Atom (V "one")))

t4 = ("let one = I 1 in \n\
      \ let inc = plus_int one in \n \
      \ let inced = map inc list in \n \
      \ let list = Cons one inced in \n \
      \ let element1 = head inced in \n \
      \ let tail1 = tail inced in \n \
      \ let element2 = head tail1 in \n \
      \ let element3 = plus_int element1 element2 in \n \
      \ case element3 of \n \
      \   I z -> #print_int z;",
      letExpr,
      Let "one" (CON (Con "I" [L 1])) (Let "inc" (THUNK (FuncCall "plus_int" [V "one"])) (Let "inced" (THUNK (FuncCall "map" [V "inc",V "list"])) (Let "list" (CON (Con "Cons" [V"one",V "inced"])) (Let "element1" (THUNK (FuncCall "head" [V "inced"])) (Let "tail1" (THUNK (FuncCall "tail" [V "inced"])) (Let "element2" (THUNK (FuncCall "head" [V "tail1"])) (Let "element3" (THUNK (FuncCall "plus_int" [V "element1",V "element2"])) (Case (V "element3") [AltCase "I" ["z"] (Atom . P $ (Primop "#print_int" [V "z"]))]))))))))
  )

t5 = ("plus_int = \\ (x1 Boxed) (y1 Boxed) -> case x1 of \n\
        \ I a1 -> case y1 of\n\
        \ I b1 -> let c1 = I (#plus a1 b1)\n\
        \ in c1;;",
      funDef,
      ("plus_int",FUNC (Fun [("x1",Boxed),("y1",Boxed)] (Case (V "x1") [AltCase "I" ["a1"] (Case (V "y1") [AltCase "I" ["b1"] (Let "c1" (CON (Con "I" [P (Primop "#plus" [V "a1",V "b1"])])) (Atom (V "c1")))])])))
      )


t6 = ("data Int = I Ival' |",
      conDecl,
      ConDecl "Int" [ConDefn {conName = "I", conTag = 0, conFields = [("Ival'",Boxed)]}]
  )



t7 = ("data Pair = Pa P_fst P_snd |", conDecl, ConDecl "Pair" [ConDefn {conName = "Pa", conTag = 0, conFields = [("P_fst", Boxed),("P_snd", Boxed)]}])

t8 = ("data List = Cons C_element C_next | Nil |", conDecl, ConDecl "List" [ConDefn {conName = "Cons", conTag = 0, conFields = [("C_element",Boxed),("C_next",Boxed)]},ConDefn {conName = "Nil", conTag = 1, conFields = []}])

t9 = ("data Unit = Unit |", conDecl, ConDecl "Unit" [ConDefn {conName = "Unit", conTag = 0, conFields = []}]
 )

t10 = ("data Bool = True | False |", conDecl, ConDecl "Bool" [ConDefn {conName = "True", conTag = 0, conFields = []},ConDefn {conName = "False", conTag = 1, conFields = []}])

t11 = ("data Int = I I_valB | \n\
\data Pair = Pa P_fst P_snd |  \n\
\data List = Cons C_element C_next | Nil | \n\
\data Unit = Unit | \n\
\data Bool = True | False | \n\
\ plus_int = \\ (x1 Boxed) (y1 Boxed) -> case x1 of  \n\
 \      I a1 -> case y1 of  \n\
  \                I b1 -> let c1 = I (#plus a1 b1) in c1;; \n\
\", program, Program [ConDecl "Int" [ConDefn {conName = "I", conTag = 0, conFields = [("I_valB",Unboxed)]}],ConDecl "Pair" [ConDefn {conName = "Pa", conTag = 0, conFields = [("P_fst",Boxed),("P_snd",Boxed)]}],ConDecl "List" [ConDefn {conName = "Cons", conTag = 0, conFields = [("C_element",Boxed),("C_next",Boxed)]},ConDefn {conName = "Nil", conTag = 1, conFields = []}],ConDecl "Unit" [ConDefn {conName = "Unit", conTag = 0, conFields = []}],ConDecl "Bool" [ConDefn {conName = "True", conTag = 0, conFields = []},ConDefn {conName = "False", conTag = 1, conFields = []}]] [("plus_int",FUNC (Fun [("plus_int_x1",Boxed),("plus_int_y1",Boxed)] (Case (V "plus_int_x1") [AltCase "I" ["plus_int_a1"] (Case (V "plus_int_y1") [AltCase "I" ["plus_int_b1"] (Let "plus_int_c1" (CON (Con "I" [P (Primop "#plus" [V "a1",V "b1"])])) (Atom (V "plus_int_c1")))])])))])

t12 = ("data Int = I' I_valB | \n\
\data Pair = Pa P_fst P_snd | \n\
\data List = Cons C_element C_next | Nil |\n\
\data Unit = Unit |\n\
\data Bool = True | False | \n\
\  \n\
\plus_int = \\ (x1 Boxed) (y1 Boxed) -> case x1 of \n\
\      I' a1 -> case y1 of \n\
\                 I' b1 -> let c1 = I' (#plus a1 b1) in c1;; \n\
\", program, Program [ConDecl "Int" [ConDefn {conName = "I'", conTag = 0, conFields = [("I_valB",Unboxed)]}],ConDecl "Pair" [ConDefn {conName = "Pa", conTag = 0, conFields = [("P_fst",Boxed),("P_snd",Boxed)]}],ConDecl "List" [ConDefn {conName = "Cons", conTag = 0, conFields = [("C_element",Boxed),("C_next",Boxed)]},ConDefn {conName = "Nil", conTag = 1, conFields = []}],ConDecl "Unit" [ConDefn {conName = "Unit", conTag = 0, conFields = []}],ConDecl "Bool" [ConDefn {conName = "True", conTag = 0, conFields = []},ConDefn {conName = "False", conTag = 1, conFields = []}]] [("plus_int",FUNC (Fun [("plus_int_x1",Boxed),("plus_int_y1",Boxed)] (Case (V "plus_int_x1") [AltCase "I'" ["plus_int_a1"] (Case (V "plus_int_y1") [AltCase "I'" ["plus_int_b1"] (Let "plus_int_c1" (CON (Con "I'" [P (Primop "#plus" [V "a1",V "b1"])])) (Atom (V "plus_int_c1")))])])))])

t13 = ("head = \\ (l Boxed) -> case l of\n\
\      Cons v n -> v\n\
\      Nil -> exception;\n\
\tail = \\ (l Boxed) -> case l of\n\
\ Cons v n -> v\n\
\ Nil -> exception;\n\
\\n\
\", many1 funDef, [("head",FUNC (Fun [("l",Boxed)] (Case (V "l") [AltCase "Cons" ["v","n"] (Atom (V "v")),AltCase "Nil" [] (Atom (V "exception"))]))),("tail",FUNC (Fun [("l",Boxed)] (Case (V "l") [AltCase "Cons" ["v","n"] (Atom (V "v")),AltCase "Nil" [] (Atom (V "exception"))])))])

t14 = ("head = \\ (l Boxed) -> case l of\n\
\      Cons v n -> v\n\
\      Nil m n -> x;\n\
\", many1 funDef, [("head",FUNC (Fun [("l",Boxed)] (Case (V "l") [AltCase "Cons" ["v","n"] (Atom (V "v")),AltCase "Nil" ["m","n"] (Atom (V "x"))])))])

data T = forall a. (Eq a, Show a) => T (String, Parsec String () a, a)

cases :: [T]
cases = [T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10, T t11, T t12, T t13, T t14]


test :: Int -> T -> IO ()
test i (T (dat, p, expected)) = case parse p "t" dat of
   Right res -> if res /= expected then error (show res) else putStrLn $ "Passed " ++ show i
   Left e -> error . show $ e


runTestSuite = zipWithM_ test [1..] cases