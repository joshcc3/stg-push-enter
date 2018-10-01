module Utils where

s :: String -> [String] -> String
s [] args = []
s ('$':'$':rest) args = (head args) ++ s rest (tail args)
s (z:rest) args = z:s rest args
