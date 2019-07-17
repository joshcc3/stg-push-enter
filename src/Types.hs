{-# LANGUAGE TemplateHaskell #-}

module Types where

import Control.Monad.State
import Data.Map
import Control.Lens
import Data.Functor.Identity
import Control.Monad.Fail

type Statement = String

type Type = String 

data C_TopLevel = 
    C_Fun String [Statement]
  | C_Struct String [Statement]
  | C_Var String [Statement]
  | C_Import [Statement]  deriving (Eq, Ord, Show)
makePrisms ''C_TopLevel


data ValueType = Boxed | Unboxed deriving (Eq, Ord, Show)
type Bindings = Map String Int
data LayoutEntry = LayoutEntry { _leSize :: String, _leIsPtr :: Bool,  _leOffset :: String } deriving (Eq, Ord, Show, Read)
type Layout = [LayoutEntry] 

type CurFun = String
data FunInfoTable = FInf {
      _finfTableName :: Maybe String,
      _finfName :: String,
      _finfArity :: Int,
      _finfArgs :: [(String, ValueType)],
      _finfLayout :: Layout
    } deriving (Eq, Ord, Show)
makeLenses ''FunInfoTable                  
type FunMap = Map String FunInfoTable
type ConMap = Map String ConstructorDefn
type FreshNameSource = Int
    
data ConstructorDefn = ConDefn {
      conName :: String,
      conTag :: Int,
      conFields :: [(String, ValueType)]
    } deriving (Eq, Ord, Show)

type MonStack = State Env

-- All names must be globally unique
data Env = Env { _funMap :: FunMap, _curFun ::  Maybe CurFun, _freshNameSource :: FreshNameSource, _conMap :: ConMap, _deferred :: [MonStack C_TopLevel], _funProtos :: [C_TopLevel], _stringBindings :: Map String Int, _liveVars :: Map String String } 
makeLenses ''Env


    
-- name, tag, fields
data ConDecl = ConDecl String [ConstructorDefn] deriving (Eq, Ord, Show)


type FunDef = (String, Object)
data Program = Program [ConDecl] [FunDef] deriving (Eq, Ord, Show)

data Object = THUNK Expression | FUNC Function | CON Constructor | BLACKHOLE | PAP PartialApp deriving (Eq, Ord, Show)

type Var = String
data Primop = Primop String [Atom] deriving (Eq, Ord, Show)
data Atom = L Literal | V Var | P Primop deriving (Eq, Ord, Show)
type Literal = Int             
data Expression = 
    Atom Atom
        | Let String Object Expression
        | Case Atom [Alt]
        | FuncCall String [Atom]
          deriving (Eq, Ord, Show)

data Alt = AltCase String [Var] Expression | AltForce String Expression deriving (Eq, Ord, Show)

type Arg = (String, ValueType)
data Function = Fun [Arg] Expression deriving (Eq, Ord, Show)
data Constructor = Con String [Atom] deriving (Eq, Ord, Show)
data PartialApp = Pap String [Atom] deriving (Eq, Ord, Show)


instance MonadFail Identity where
  fail x = error x
