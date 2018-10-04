module Types where

import Control.Monad.State
import Data.Map

type Statement = String

data C_TopLevel =
    C_Fun String [Statement] [C_TopLevel]
  | C_Struct String [Statement] [C_TopLevel]
  | C_Var String [Statement]

type Bindings = Map String Int
type CurFun = String
data FunInfoTable = FInf String Int [(String, ValueType)]
type FunMap = Map String FunInfoTable
type ConMap = Map String ConstructorDefn
type FreshNameSource = Int
    
data Env = Env FunMap CurFun FreshNameSource ConMap

type MonStack = State Env

-- name, tag, fields
data ValueType = Boxed | Unboxed
data ConstructorDefn = ConDefn { conName :: String, conArity :: Int, conFields :: [(String, ValueType)] }
data ConDecl = ConDecl String [ConstructorDefn]

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

data Function = Fun [(String, ValueType)] Expression
data Constructor = Con String [Atom]
data PartialApp = Pap String [Atom]
