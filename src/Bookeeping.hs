
module Bookeeping where

import Types
import Utils
import qualified Data.Map as M
import Control.Monad.State
import Control.Lens
import Control.Applicative
import CConstructs

freshInt :: MonStack Int
freshInt = do
  x <- use freshNameSource
  freshNameSource += 1
  return x

freshName :: MonStack String
freshName = do
  x <- freshInt
  return (to_temp_var x)

freshIntStream :: Int -> MonStack [Int]
freshIntStream z = do
  st <- get
  result <- stream
  put st
  freshNameSource += z
  return (take z result)
      where
        stream = (:) <$> freshInt <*> stream
