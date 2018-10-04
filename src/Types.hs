{-# LANGUAGE TemplateHaskell #-}

module Types where

import Control.Monad.State
import Data.Map
import Control.Lens

type Statement = String

data C_TopLevel = 
    C_Fun String [Statement] [C_TopLevel]
  | C_Struct String [Statement] [C_TopLevel]
  | C_Var String [Statement] deriving (Eq, Ord, Show)

type Bindings = Map String Int
type CurFun = String
data FunInfoTable = FInf String Int [(String, ValueType)] deriving (Eq, Ord, Show)
type FunMap = Map String FunInfoTable
type ConMap = Map String ConstructorDefn
type FreshNameSource = Int
    
data ValueType = Boxed | Unboxed deriving (Eq, Ord, Show)
data ConstructorDefn = ConDefn { conName :: String, conArity :: Int, conFields :: [(String, ValueType)] } deriving (Eq, Ord, Show)

data Env = Env { _funMap :: FunMap, _curFun ::  CurFun, _freshNameSource :: FreshNameSource, _conMap :: ConMap } deriving (Eq, Ord, Show)
makeLenses ''Env

type MonStack = State Env
    
-- name, tag, fields
data ConDecl = ConDecl String [ConstructorDefn] deriving (Eq, Ord, Show)

type Program = [(String, Object)]
data Object = THUNK Expression | FUNC Function | CON Constructor | BLACKHOLE | PAP PartialApp deriving (Eq, Ord, Show)

type Var = String
data Atom = L Literal | V Var deriving (Eq, Ord, Show)
data Literal = I Int             deriving (Eq, Ord, Show)
data Expression = 
    Atom Atom
        | Let String Object Expression
        | Case Atom [Alt]
        | Primop String [Atom]
        | FuncCall String [Atom]
          deriving (Eq, Ord, Show)

data Alt = AltCase String [Var] Expression | AltForce String Expression deriving (Eq, Ord, Show)

type Arg = (String, ValueType)         
data Function = Fun [Arg] Expression deriving (Eq, Ord, Show)
data Constructor = Con String [Atom] deriving (Eq, Ord, Show)
data PartialApp = Pap String [Atom] deriving (Eq, Ord, Show)
