stg-push-enter
--------------

Implementation of a compiler for an [stg-like](https://ghc.haskell.org/trac/ghc/wiki/Commentary/Compiler/GeneratedCode) language
using the `push-enter` approach based on [this paper](http://simonmar.github.io/bib/papers/evalapplyjfp06.pdf).

# Features
 - Lazy Evaluation
 - Currying
 - Higher order functions
 - Unboxed values
 - Simple ADTs
 - Lambda Functions
 - Tail-call Optimization
 - Stack splitting (TODO)
 - Garbage Collection (TODO)
 - Typeclasses (TODO)
 - STM (TODO)
 - FFI (TODO)
 - System F type checking (TODO)
 - Stack Traces (TODO)
  
# Examples

Given the following prelude:
```
data Int = I IB |
data Unit = Unit |
data List = Cons V List | Nil |

zipwith = \ (f Boxed) (l1 Boxed) (l2 Boxed) -> case l1 of
  Cons x y -> case l2 of 
                Cons a b -> let new_elem = f x a in
                            let new_tail = zipwith f y b in
                            let zipped = Cons new_elem new_tail in
                            zipped;
                Nil -> let res = Nil in res
  Nil -> let res = Nil in res;

map = \ (f  Boxed) (l  Boxed) -> case l of
   Cons x y -> let new_elem = f x in
               let new_tail = map f y in
               let res = Cons new_elem new_tail in
               res
   Nil -> let res = Nil in res;

plus_int = \(x1  Boxed) (y1  Boxed) -> case x1 of 
      I a1 -> case y1 of 
                 I b1 -> let c1 = I (#plus a1 b1) in c1;;

print_int_list = \ (l  Boxed) -> case l of
                 Cons v n -> case v of
                               I x -> let rest = print_int_list n in
                                       let inner = seq (#print_int x) rest in
                                       let unit = Unit in
                                       seq inner unit;;

take = \ (n  Boxed) (l Boxed) -> case n of
  I x -> case x of 
    0 -> let res = Nil in res
    n2 -> case l of
           Cons h t -> let n3 = #sub n2 1 in
                      let n4 = I n3 in
                      let rest = take n4 t in
                      let res = Cons h rest in
                      res ;;;

seq = \ (a  Boxed) (b Boxed) -> case a of
                x -> b;
```

Below is the famous infinite fibonacci sequence.
```
main_ = let one = I 1 in 
      let fibs = Cons one fibtail in
      let fibtail = Cons one fibrest in
      let fibrest = zipwith plus_int fibs fibtail in
      print_int_list fibs
```
The above prints
```
1
1
2
3
5
8
...
```

# Language Spec
See the paper (page 4, 5) for a more detailed description of the constructs.
```
atom:  a1 .. an


Variables
var := fn, x, y

Literals
lit := var | i | d (unboxed integer/double)

alts := C x1 x2 .. xn -> e | x -> e

Expressions
e := case x of { alts* } | 
     fn a1 .. an | 
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

I've changed the grammar slightly and added `primops` to an `atom`. This is to allow them to be used in constructors/function calls, etc. Unboxed types are not allowed to be lifted values (values that can evaluate to bottom - thunks mainly) so you can't instantiate them in a let binding.

# Compile an STG program
Write an STG program and save it in `<file>.stg`, then run `./compile <path>` - (TODO)

The parser frontend is unimplemented, currently.
Instead, prepare your program in the edsl by creating a value of type `Program`. Then you run `compile` on the value also passing in the c output file name. This generates a `.c` file in `c_out/out` and a `.sh` file in `c_out/`. The `.sh` file compiles the generated C code against my `rts` library and produces a binary in `c_out/`.



# Details
##  Memory Representation of Heap Objects
The memory representation definitions are inside `c_out/rts/static.h`. All heap objects consist of an info table pointer followed by a payload, the descriptions of which are given in the paper.
  - `Constructor: (<info table pointer>, <arg1> ... ) ` where the args can be either boxed or unboxed values. The layout is specified in the info table.
  - `Thunk: (<info table pointer>, <hash map pointer>) ` the hash map points to the free variables inside the body of the thunk
  - `Fun(<info table pointer>, arity)`
  - `PAP(<info table pointer to function>, arity?)`
  - `BLACKHOLE: (<info ptr to thunk>)`

A `PAP` heap objects contain the info pointer and then the arguments. We use the info pointer to infer the size and type of each argument.
The `Fun` object stores the offset from the start, if all the arguments were laid out consecutively.

I haven't implemented 1 optimization mentioned in the paper - in the info table, the code is juxtaposed against the data. An info table pointer, will point to the code of the object. The rest of the data of the info table will be laid out above the code and can be accessed by negative references. the fast entry point lies just below the slow entry point because all the slow entry point does is do the satisfaction check, prepare the arguments and then call the fast entry point. This optimization means we perform 1 dereference instead of 2 when evaluating an object.

## Runtime Memory
Effectively we have two stacks. One for the c runtime and the other is the stack we use for pushing continuations.

The stack pointer starts of at the byte just after the highest point in the stack.

### Issues with unboxed args to constructors
A constructor that takes an unboxed value can never take a ref so if you're doing a `let x = I# int_value` the code will evaluate its argument strictly because of course we can't store a ref as an argument to the I# constructor.

### Issues with update continuations
In order to perform the 'Update' continuation we cannot just pass around pointers to the heap object.
This is because when attempting to update a heap object you'll have to go through all the bindings maps (that map a bound variable to the heap object  - represented as maps from int -> ptr)) and update the reference to the heap object in them. So instead we refer to heap objects via references. A reference is just a pointer to an object where all writes to the reference are just writes to the underlying object.
One alternative is to maintain a pointer table and pass around pointers into the pointer table and update entries in the pointer table when updating through an Update continuation.
Another alternative is to maintain a global bindings map - then the key of the map would have to be globally unique and you would pass around the keys to the object. This is basically the same as the pointer table approach with some added overhead.
My decision was to maintain a pointer table and have the local global bindings point to the pointer table references referred to in the pointer table. Requesting a new reference adds an entry into the pointer table. Adding a mapping from to a hashmap
maps the key to the reference in the table.
Using pointers internal to a structure is a terrible idea (in this case on resize all your pointers would be invalidated) so you store them as offsets from the start of the index.



## Code flow
### Examples
Examples of the flow for `head :: [a] -> a`
```
ref head_case_cont(ref bindings)
{
    arg = get l from bindings
    if(constructor)
    {
      must be cons
      return ref to x in l
    }
    else
    {
      must be a thunk
      push the case frame refed to case_cont
      push the update frame to update l
      pop case(pop update(enter thunks code))
    }
}
/*
    head :: [a] -> a
    head l = case l of
                Cons x xs -> x
*/
ref head_fast(ref list)
{
    pop arg from stack
    bind l to arg in bindings
    head_case_cont(bindings)
}


ref head_slow(ref null)
{
    // no need to perform an argument satisfaction check because the only way this is called is when its applied to its argument
    pop arg from stack
    head_fast(arg)
}

```

`maybe :: Maybe a -> (a -> b) -> b -> b` 

#### Fast entry point
```  
  if the argument is a constructor
    if its a Just:
      perform the function analysis case
      return the function applied to the
    else return nothing;

  otherwise it must be a thunk
    push the case frame
    push update frame
    set the ref to a blackhole
    enter the thunk
    pop the update continuation
    pop the case continuation
```

### Why do we need continuations?
They're kind of like return addresses with packed info. Since I compile to C, they can be used to implement tail call optimization although I haven't figured out how yet. They are also needed because the same sections of code are visited from different points.  (like after a case or after a thunk). 

# Optimizations

## Tail Call Optimization
In the current code (2018-10-11) all tail calls generate a new C stack frame which segfaults the program. All tail calls can be safely optimized to jumps.
A simple way (hacky?) way to implement tail call opt:
You
```
int f(int x)
{
  return g(x++);
}

// use the sysv_abi to ensure compatibility with windows, amd
int f_opt(int x) __attribute__ ((noinline, noclone, sysv_abi));
int f_opt(int x)
{
  x++;
  asm volatile (
		     "movq %%rbp, %%rsp;\n\t"
		     "popq %%rbp;\n\t"
		     "movl %0, %%edi;\n\t"
		     :
		     :"r" (x));
		     :"edi"
  goto *(void*)f;

}

// 261945 - and then it segfaults (4MB of stack space)
root@9c9ccbe47a52:/code/c_out/rts/src/experiments# ./a.out 
0
100000
200000
Segmentation fault 

// For f_opt, printing every 10^5th iteration I get:
.
.
.
1808400000
1808500000
1808600000
^C
```

When implementing tail call optimization - remember to tell gcc that you're clobbering some registers. Also you cant refer to variables after you pop the stack.
When we have to jump to an anonymous functions entry then we have to perform the jump inside assembly rather than outside like I normally do - this is because you can't refer to variables on the stack to find the location after you've popped the stack frame.
All tail calls have now been replaced with jmps. The program ends when there are no more continuations on the stack.

Finally added tail call optimization everywhere:
.
.
.
599182
599183
599184
599185
599186
Haven't added stack growth yet because we need to learn to walk first: Success
a.out: rts/src/stg/plus_int/stack.c:104: push_case_frame: Assertion `0' failed.
Aborted

## Others

 - Single argument functions don't need to perform an argument satisfaction check - the function call rules would never apply for that to be the case.

 - Another optimization that we follow from the paper is to limit the number of su saves. As the paper describes, the only time we care about saving su is when not saving it would mean that the scrutinee of the case or update frame would get its argument satisfaction check wrong (i.e. the scrutinee evaluates to a partial function that mistakes the number of arguments available because we haven't saved the closes stack frame).
 Thus we only care about this when the scrutinee evaluates to a function.
 Since it's rare for the scrutinee of a case expression to evaluate to a function we never save su for stack frames. In the odd case (like for `seq`) we push an update frame that updates su on top of the case frame.



# Dev environment
Clone the project.
docker run -v <source path>:/code: -it ubuntu /bin/bash.
Edit in intellij locally, happy days

