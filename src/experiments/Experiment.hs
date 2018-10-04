{-# LANGUAGE TemplateHaskell #-}

import qualified Data.Map as M
import Control.Monad
import Control.Monad.State
import Control.Lens
    



data St = St { _a :: M.Map Int Int, _b :: String } deriving Show
makeLenses ''St

function :: State (M.Map Int St) ()
function = zoom (ix 1) $
           a.ix 3 += 10

f2 :: State St String
f2 = use (b)

  
  
  
