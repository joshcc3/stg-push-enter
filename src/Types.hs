module Types where

type Program = [(String, Object)]
data Object = THUNK Expression | FUNC Function | CON Constructor | BLACKHOLE | PAP PartialApp

type Var = String
data Atom = L Literal | V Var
data Literal = I Int            
data Expression =
    Atom Atom
        | Let String Object Expression
        | Case Atom [Alt]
        | Primop String [Atom]
        | FuncCall String [Atom]

data Alt = AltCase String [Var] Expression | AltForce String Expression

data Function = Fun [String] Expression
data Constructor = Con String [Atom]
data PartialApp = Pap String [Atom]
