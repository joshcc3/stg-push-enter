
{-g = let x = 1:y in
       let y = map (+1) x in
       print (take 10 x)
  -}             
g = print (take 10 x)
    where
      x = 1:(let z = [1] in (let j = z in j))
      y = map (+1) x
      
               
