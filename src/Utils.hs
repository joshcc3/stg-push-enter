module Utils where

s :: Show a => String -> [String    ] -> String
s [] args = []
s '$':'$':rest args = show (head args) ++ s rest (tail args)
s z:rest args = z:s rest args