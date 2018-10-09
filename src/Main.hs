module Main where

import CodeGen
import Types
import Utils
import SampleProgram

{-
given a program and outfile (without extension):
      it gens the statements
      writes it out to a file outfile in c_out/out
      and then writes out the compile instruction to c_out/compile_<outfile>.sh
-}
compile :: Program -> String -> IO ()
compile prog outfile = do
  let content = toProgram prog
      compile_string = s "gcc  -Wall -ggdb -I rts/src/ -I . out/$$.c rts/src/stg/plus_int/stack.c rts/src/stg/plus_int/static.c rts/src/stg/heap.c rts/src/data/string_.c rts/src/containers/mmanager.c rts/src/containers/arraylist.c rts/src/stg/bindings.c rts/src/stg/math.c rts/src/typeclasses.c rts/src/stg/util.c rts/src/containers/llist.c rts/src/containers/hash_set.c rts/src/containers/resizable_array.c rts/src/containers/hash_map.c\n" [outfile]
  writeFile (s "../c_out/out/$$.c" [outfile]) (content)
  writeFile (s "../c_out/compile_$$.sh" [outfile]) compile_string
    
pprog = putStrLn . toProgram
