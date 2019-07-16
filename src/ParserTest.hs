module ParserTest where

import Parser
import Text.Parsec
import Types
import Text.Parsec.Token


t1 = ("1", lit, L 1)

t2 = ("case x of\n  Pair a b -> +# a b",
      caseExpr,
      Case (V "x") [AltCase "Pair" ["a","b"] (Atom (P (Primop "+#" [V "a",V "b"])))]
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
      \   I z -> print_int z",
      letExpr,
      Let "one" (CON (Con "I" [L 1])) (Let "inc" (CON (Con "plus_int" [V "one"])) (Let "inced" (CON (Con "map" [V "inc",V "list"])) (Let "list" (CON (Con "Cons" [V "one",V "inced"])) (Let "element1" (CON (Con "head" [V "inced"])) (Let "tail1" (CON (Con "tail" [V "inced"])) (Let "element2" (CON (Con "head" [V "tail1"])) (Let "element3" (CON (Con "plus_int" [V "element1",V "element2"])) (Case (V "element3") [AltCase "I" ["z"] (FuncCall "print_int" [V "z"])])))))))))

t5 = ("plus_int = \\ (x1 Boxed) (y1 Boxed) -> case x1 of \n\
        \ I a1 -> case y1 of\n\
        \ I b1 -> let c1 = I (+# a1 b1)\n\
        \ in c1",
      funDef,
      ("plus_int",FUNC (Fun [("x1",Boxed),("y1",Boxed)] (Case (V "x1") [AltCase "I" ["a1"] (Case (V "y1") [AltCase "I" ["b1"] (Let "c1" (CON (Con "I" [P (Primop "+#" [V "a1",V "b1"])])) (Atom (V "c1")))])])))
      )


t6 = ("data Int = I Ival' |", 
      conDecl,  
      ConDecl "Int" [ConDefn {conName = "I", conTag = 0, conFields = [("Ival'",Boxed)]}]
  )



t7 = ("data Pair = Pa P_fst P_snd |", conDecl, ConDecl "Pair" [ConDefn {conName = "Pa", conTag = 0, conFields = [("P_fst",Unboxed),("P_snd",
Unboxed)]}])

t8 = ("data List = Cons C_element C_next | T x |", conDecl, ConDecl "List" [ConDefn {conName = "Cons", conTag = 0, conFields = [("C_element",Unboxed),("C_next",Unboxed)]},ConDefn {conName = "T", conTag = 1, conFields = [("x",Unboxed)]}])

t9 = ("data Unit = Unit |", conDecl, ConDecl "Unit" [ConDefn {conName = "Unit", conTag = 0, conFields = []}]
 )

t10 = ("data Bool = True | False |", conDecl, ConDecl "Bool" [ConDefn {conName = "True", conTag = 0, conFields = []},ConDefn {conName = "False", conTag = 1, conFields = []}])


t11 = "data Int = I' I_val | \n\
\data Pair = Pa P_fst P_snd |  \n\
\data List = Cons C_element C_next | Nil | \n\
\data Unit = Unit | \n\
\data Bool = True | False | \n\
\ plus_int = \\ (x1 Boxed) (y1 Boxed) -> case x1 of  \n\
 \      I' a1 -> case y1 of  \n\
  \                I' b1 -> let c1 = I' (+# a1 b1) in c1 \n\
\"

t12 = "data Int = I' I_val | \n\
\data Pair = Pa P_fst P_snd | \n\
\data List = Cons C_element C_next | Nil |\n\
\data Unit = Unit |\n\
\data Bool = True | False | \n\
\  \n\
\plus_int = \\ (x1 Boxed) (y1 Boxed) -> case x1 of \n\
\      I' a1 -> case y1 of \n\
\                 I' b1 -> let c1 = I' (+# a1 b1) in c1 \n\
\"

t13 = "head = \\ (l Boxed) -> case l of\n\
\      Cons v n -> v\n\
\      Nil -> exception\n\
\\n\
\\n\
\\n\
\tail = \\ (l Boxed) -> case l of\n\
\ Cons v n -> v\n\
\ Nil -> exception\n\
\\n\
\"