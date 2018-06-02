stg-push-enter
--------------

Implementation of a compiler
 for an [stg](https://ghc.haskell.org/trac/ghc/wiki/Commentary/Compiler/GeneratedCode)-like language
using the push-enter approach based on [this paper](http://simonmar.github.io/bib/papers/evalapplyjfp06.pdf).

# Language Spec
See the paper (page 4, 5) for a more detailed description of the constructs.
```
Variables
var := fn, x, y

Literals
lit := var | i | d (unboxed integer/double)

alts := C x1 x2 .. xn -> e | x -> e

Expressions
e := case x of { alts* } | 
     fn a1 .. an | 
     primop a1 .. an | 
     lit 

Heap objects
obj := FUNC(x1 x2 .. xn -> e) |
       PAP(f x1 x2 .. xn) | 
       BLACKHOLE |
       THUNK(e) |
       CONC( C x1 x2 .. xn)
    
Programs   
prog := f1 = obj1; f2 = obj2; f3 = obj3 ...

```






