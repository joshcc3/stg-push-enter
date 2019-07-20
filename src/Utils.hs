module Utils where

s :: String -> [String] -> String
s [] args = []
s ('$':'$':rest) args = (head args) ++ s rest (tail args)
s (z:rest) args = z:s rest args


alX (Just x) = x
alX Nothing = error "You showed him"

alWithCtx ctx (Just x) = x
alWithCtx ctx Nothing = error $ show ctx

al (Just x) = x

startsWith a s = length a < length s && all (uncurry (==)) (zip a s)

whileM :: Monad m => m Bool -> m a -> m [a]
whileM cond m = go []
    where
      go xs = do
        c <- cond
        if c
        then m >>= \x -> go (x:xs)
        else return $ reverse xs
