

eval (L (I x)) = show x
eval (V x) = x
eval (Let s o e) = do
            ref <- createThunk "bindings" o
            bind s ref
            eval e
eval (Case e alts) = do
  pushCaseFrame contName updateid "bindings"
  returnCaseCont e alts
eval (Primop "+#" [L (I x), L (I y)]) = show (x + y)
eval (Primop "+#" [V a, z]) = eval (Case (V a) alts)
    where
      alts = [Alt "I#" [newVar] 
            
