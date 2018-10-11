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
      compile_string = s "gcc  -std=c11 -Wall -ggdb -I rts/src/ -I . out/$$.c rts/src/stg/plus_int/stack.c rts/src/stg/plus_int/static.c rts/src/stg/heap.c rts/src/data/string_.c rts/src/containers/mmanager.c rts/src/containers/arraylist.c rts/src/stg/bindings.c rts/src/stg/math.c rts/src/typeclasses.c rts/src/stg/util.c rts/src/containers/llist.c rts/src/containers/hash_set.c rts/src/containers/resizable_array.c rts/src/containers/hash_map.c\n" [outfile]
      cout = s "../c_out/out/$$.c" [outfile]
      compile_script = s "../c_out/compile_$$.sh" [outfile]
  putStrLn $ s "Compiling to $$" [cout]
  writeFile cout content
  putStrLn $ s "Writing compile script to $$" [compile_script]
  writeFile compile_script compile_string
  putStrLn $ s "Compilation of $$ complete." [outfile]
  putStrLn ""

pprog = putStrLn . toProgram

main = do
  mapM (uncurry compile) testSuite
