module Utils where

s :: String -> [String] -> String
s [] args = []
s ('$':'$':rest) args = (head args) ++ s rest (tail args)
s (z:rest) args = z:s rest args

al (Just x) = x
startsWith a s = length a < length s && all (uncurry (==)) (zip a s)
