stg-push-enter
--------------

Implementation of a compiler
< for an [stg](https://ghc.haskell.org/trac/ghc/wiki/Commentary/Compiler/GeneratedCode)-like language
using the push-enter approach based on [this paper](http://simonmar.github.io/bib/papers/evalapplyjfp06.pdf).

# Features
 - Lazy Evaluation
 - Currying
 - Higher order functions
 - Unboxed values
 - Simple ADTs
 - Stack Traces
 - Lambda Functions
 - Garbage Collection (TODO)
 - Typeclasses (TODO)
 - STM (TODO)
 - FFI (TODO)
 - System F type checking (TODO)
 - Tail-call Optimization (TODO)
  

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

I've changed the grammar slightly and added primops to an 'atom'. This is to allow them to be used in constructors/function calls, etc. Unboxed types are not allowed to be lifted values (values that can evaluate to bottom - thunks mainly) so you can't instantiate them in a let binding.

# Objectives
 - Manually 'compile' stg programs into C - Done
 - Write a code generator for stg - Done
 - Write a parallel generational garbage collector for the runtime based on ()
 - Implement STM for STG

# Compilation
It's currently a bit haphazard but, you prepare your program in the edsl by creating a value of type `Program` (this is basically a carbon copy of the stg structure). Then you run `compile` on the value also passing in the c output file name. This generates a `.c` file in `c_out/out` and a `.sh` file in `c_out/`. The `.sh` file compiles the generated C code against my `rts` library and produces a standalone binary in `c_out/`.



# Details
##  Memory Representation of Heap Objects
Most memory representation definitions are inside static.h. All heap objects consist of an info table pointer followed by a payload, the descriptions of which are given in the paper.
Constructor: (<info table pointer>, <arg1> ... ) where the args can be either boxed or unboxed values. The layout is specified in the info table.
Thunk: (<info table pointer>, <hash map pointer>) the hash map points to the free variables inside the body of the thunk
Fun(<info table pointer>, arity) number of arguments this takes?
PAP(<info table pointer to function>, arity?)
BLACKHOLE: (<info ptr to thunk>,)

A PAP heap objects contain the info pointer and then the arguments. We use the info pointer to infer the size and type of each argument.
The fun object stores the offset from the start if all the arguments were laid out consecutively.

In the paper they make many optimizations. For one, in the info table, the code is juxtaposed against the rest of the entries. An info table pointer, will point to the code of the object. The rest of the data of the info table will be laid out above the code and can be accessed by negative references. the fast entry point lies just below the slow entry point because all the slow entry point does is do the satisfaction check, prepare the arguments and then call the fast entry point.

Effectively we have two stacks. One for the c runtime and the other is the stack we use for pushing continuations.

The stack pointer starts of at the byte just after the highest point in the stack.

Remember to bind the actual value of the case when the case is evaluated.


TODO: Find a more elegant way of handling the binding of the case continuation result.


In order to perform the 'Update' continuation we cannot just pass around pointers to the heap object.
This is because when attempting to update a heap object you'll have to go through all the bindings maps (that map a bound variable to the heap object  - represented as maps from int -> ptr)) and update the reference to the heap object in them. So instead we refer to heap objects via references. A reference is just a pointer to an object where all writes to the reference are just writes to the underlying object.
One alternative is to maintain a pointer table and pass around pointers into the pointer table and update entries in the pointer table when updating through an Update continuation.
Or you could maintain a global bindings map - then the key of the map would have to be globally unique and you would pass around the keys to the object. This is basically the same as the pointer table approach with some added overhead.
My decision was to maintain a pointer table and have the local global bindings point to the pointer table references referred to in the pointer table. Requesting a new reference adds an entry into the pointer table. Adding a mapping from to a hashmap
maps the key to the reference in the table.
Using pointers internal to a structure is a terrible idea (in this case on resize all your pointers would be invalidated) so you store them as offsets from the start of the index.
One tricky question is how to maintain the pointer table.. evidently its append only and only the garbage collector can really "compact it". 

A constructor that takes an unboxed value can never take a ref so if you're doing a `let x = I# int_value` the code will evaluate its argument strictly because of course we can't store a ref as an argument to the I# constructor.

// TODO need to implement the slow entry point for functions


## C Functions:
When a case is being evaluated and the only options are case constructors:
The arguments can only ever evaluate to thunks or the case constructor (not func/pap)

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

`maybe :: Maybe a -> (a -> b) -> b -> b` function
  fast entry point:
  
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

### Notes
 - I do lots of stupid things like retrieve arguments from the bindings map which are already in scope - this is mainly because it's difficult to distinguish the cases. Would be worth adding a component to `Env` which tracks the live variables which you can lookup and which you have to refer to the global map for.
 - The case continuation updates a particular binding it knows are not among the cases free variables so that the actual continuation can pluck the referenced object out at that binding.

 - Single argument functions don't need to perform an argument satisfaction check - the function call rules would never apply for that to be the case.

 - Another optimization that we follow from the paper is to limit the number of su saves. As the paper describes, the only time we care about saving su is when not saving it would mean that the scrutinee of the case or update frame would get its argument satisfaction check wrong (i.e. the scrutinee evaluates to a partial function that mistakes the number of arguments available because we haven't saved the closes stack frame).
 Thus we only care about this when the scrutinee evaluates to a function.
 Since it's rare for the scrutinee of a case expression to evaluate to a function we never save su for stack frames. In the odd case (like for `seq`) we push an update frame that updates su on top of the case frame.

### Why do we need continuations?
They're kind of like return addresses with packed info. Since I compile to C, they can be used to implement tail call optimization although I haven't figured out how yet. They are also needed because the same sections of code are visited from different points.  (like after a case or after a thunk). 



# Dev environment
Clone the project.
docker run -v <source path>:/code: -it ubuntu /bin/bash.
Edit in intellij locally, happy days


# Notes about C
`malloc` is contained in `stdlib.h`.

Edit: The reason for the below is probably that the compiler doesn't carry around the type of the struct and just rewrites accessors as offsets.
You can only use the brackets initializer for structs at variable declaration time. You can't reassign with a bracket initializer. I'm confused by this, looking at the asm generated:
```
struct a {
  int a;
  int b;
};

int main()
{
    struct a x = { .a = 10, .b = 123 };
}
```
gives:
```
main:
.LFB0:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -8[rbp], 10
	mov	DWORD PTR -4[rbp], 123
	mov	eax, 0
	pop	rbp
	ret
```

It looks like bracket initializers refer the variable via its rbp offset so surely reassigning with a bracket initializer should be equally straightforward?


## Structs are values example:
```
struct a {
  int a;
  int b;
};

int main()
{
    struct a x = { .a = 10, .b = 123 };
    struct a y;
    y = x;
    y.a = 345;

}
```

gives:
```
main:
.LFB0:
	push	rbp
	mov	rbp, rsp
	mov	DWORD PTR -16[rbp], 10
	mov	DWORD PTR -12[rbp], 123
	mov	rax, QWORD PTR -16[rbp]
	mov	QWORD PTR -8[rbp], rax
	mov	DWORD PTR -8[rbp], 345
	mov	eax, 0
	pop	rbp
	ret
```
The compiler optimizes the copying of x to y as a single QWORD load from x to y and then updates the (DWORD)b part of y.

When a struct contains example 3 elements, it'll break it down into a series of QWORD loads ending with the corresponding size.

## Macros
Macros are created using #define. The gcc pages are a good source but a quick overview:
#define A 1
replaces A with 1
Macro bindings are sequential and 'recursive' (the result of an expansion is also evaluated) so
```
#define A 1
#define B A
B
#define A 2
B
```
would yield
```
1
2
```

You can pass arguments to macros:
```
#define A(x, y) x + y
```
It's replaced only if the token matches the argument name so
```
#define A(x) x_ref // no effect
```

Multiline macros are resolved like so:
```
#define GET_BINDING(ref_name, val_type, val_name, binding_key, bindings) ref ref_name;\
      get_binding(bindings, binding_key, &ref_name); \
      val_type val_name = (val_type)get_ref(ref_name);

int main()
{
    ref a; get_binding(bindings, 0, &a); void** f_ref = (void**)get_ref(a);
}
```

## Lessons
Especially for arithmetic functions don't forget to assert the domain of functions as well as the range. (e.g. % division by 0)
Write out the preconditions and post-conditions first. For every loop write the invariant.
When requesting data pay close attention to whether or not it should be initialized or no (malloc vs calloc). For example: malloc'ing data for a (const void* array) might give you some uninitialized block that could be interpreted as random data. (you won't be able to distinguish
the uninitialized data).

## Pointers

### Pointer nonsense:
 - `const int *p` - pointer to a constant int
 - `int *const p` - constant pointer to an int
 - `int (*f_ptr)(int)` - pointer to a function that takes and returns an int
 - `int (*(*f_ptr)(int))(int)` - pointer to a function that takes an int and returns a pointer to a function that takes and returns an int
 - `int *p[10]` - an array of 10 pointers to ints
 - `int (*p)[10]` - a pointer to an array of 10 ints

The `const` keyword affects the thing to the left of it though it can be used either to the left or right of a type.
```
char ** p1;                    //        pointer to       pointer to       char
const char **p2;               //        pointer to       pointer to const char
char * const * p3;             //        pointer to const pointer to       char
const char * const * p4;       //        pointer to const pointer to const char
char ** const p5;              //  const pointer to       pointer to       char
const char ** const p6;        //  const pointer to       pointer to const char
char * const * const p7;       //  const pointer to const pointer to       char
const char * const * const p8; //  const pointer to const pointer to const char
```

`int *a, b` creates a pointer to an `int`, `a` and an `int`, `b`. 
Typdedefs can be used to overcome the 'applies to the variable not type' rule.

### Rules for reading declarations
Taken from [here](https://www.codeproject.com/Articles/7042/How-to-interpret-complex-C-C-declarations)

This is a simple rule that allows you to interpret any declaration. It runs as follows:

Start reading the declaration from the innermost parentheses, go right, and then go left. When you encounter parentheses, the direction should be reversed. Once everything in the parentheses has been parsed, jump out of it. Continue till the whole declaration has been parsed.

One small change to the right-left rule: When you start reading the declaration for the first time, you have to start from the identifier, and not the innermost parentheses.

Take the example given in the introduction:

```

int * (* (*fp1) (int) ) [10];
This can be interpreted as follows:

Start from the variable name -------------------------- fp1
Nothing to right but ) so go left to find * -------------- is a pointer
Jump out of parentheses and encounter (int) --------- to a function that takes an int as argument
Go left, find * ---------------------------------------- and returns a pointer
Jump put of parentheses, go right and hit [10] -------- to an array of 10
Go left find * ----------------------------------------- pointers to
Go left again, find int -------------------------------- ints.
Here's another example:


int *( *( *arr[5])())();
Start from the variable name --------------------- arr
Go right, find array subscript --------------------- is an array of 5
Go left, find * ----------------------------------- pointers
Jump out of parentheses, go right to find () ------ to functions
Go left, encounter * ----------------------------- that return pointers
Jump out, go right, find () ----------------------- to functions
Go left, find * ----------------------------------- that return pointers
Continue left, find int ----------------------------- to ints.
```


### Pointer to an array of ints
Subscripts have greater precedence than `*` so it needs to be in brackets.
```
  int (*p2)[3];

  p2 = malloc(sizeof(int)*3);

  printf("Values inside p: p2 = %p, *p2 = %p, *p2[0] = %p, p2[0] = %p, p2[0][0] = %p, p2[0][1] = %p, *p2[1] = %p, p2[1][0] = %p, p2[1][1] = %p\n", \
         p2, (*p2), &(*p2)[0], p2[0], &p2[0][0], &p2[0][1], &(*p2)[1], &p2[1][0], &p2[1][0]);

Output:
Values inside p: p2 = 0x1f16010, *p2 = 0x1f16010, *p2[0] = 0x1f16010, p2[0] = 0x1f16010, p2[0][0] = 0x1f16010, p2[0][1│
] = 0x1f16014, *p2[1] = 0x1f16014, p2[1][0] = 0x1f1601c, p2[1][0] = 0x1f1601c  
```
Notably, `p2[0]`, `*p2`, `p2`, `p2[0][0], *p2[0]` have the same address, I think they differ in how assignments work on them.

```
  int (*p2)[3];
  int p3[3][3];
  p3[0][0] = 1001;
  p2 = malloc(sizeof(int)*9);
  // Does not work - you must use constant expressions to initialize arrays, (what is a constant - I have not formal defn yet)
  // int p4[3] = *p2;


  p2[0][0] = 10;
  printf("1. Should be 10, Value at p2[0][0]%d\n", p2[0][0]);

  *p2[0] = 11;
  printf("2. Should be 11, Value at p2[0][0]%d\n", p2[0][0]);

  // Does not work - you cannot assign to an array (you can assign any other lvalue)
//  *p2 = p3[0];
//  printf("3. Should be 1001, Value at p2[0][0]%d\n", p2[0][0]);

  // Does not work for the same reason above
//  *p2 = p4;
//  printf("4. Should be 11, Value at p2[0][0]%d\n", p2[0][0]);

  *p2[0] = 12;
  printf("4. Should be 12, Value at p2[0][0]%d\n", p2[0][0]);
```

###
### 1 difference between lvalues and rvalues
```
   int *arr = malloc(sizeof(int)*10);
   printf("This is an lvalue: %p", &(*arr));
   // printf("This is an lvalue: %p", &(1));
```
Does not work because you can only reference lvalues. They are memory locations in assembly.
```
.LC0:                                                                                                                 │
        .string "This is an lvalue: %p"                                                                               │
        .text                                                                                                         │
        .globl  example0                                                                                              │
        .type   example0, @function                                                                                   │
example0:                                                                                                             │
.LFB3:                                                                                                                │
        .cfi_startproc                                                                                                │
        push    rbp                                                                                                   │
        .cfi_def_cfa_offset 16                                                                                        │
        .cfi_offset 6, -16                                                                                            │
        mov     rbp, rsp                                                                                              │
        .cfi_def_cfa_register 6                                                                                       │
        sub     rsp, 16                                                                                               │
        mov     edi, 40                                                                                               │
        call    malloc                                                                                                │
        mov     QWORD PTR [rbp-8], rax                                                                                │
        mov     rax, QWORD PTR [rbp-8]                                                                                │
        mov     rsi, rax                                                                                              │
        mov     edi, OFFSET FLAT:.LC0                                                                                 │
        mov     eax, 0                                                                                                │
        call    printf
```
Assembling to intel syntax via `gcc -S -masm=intel set.c` gives the above. 
The `&(*arr)` has been optimized away, however when trying with just `*arr` we see:
```
        .section        .rodata                                                
.LC0:                                                                          
        .string "This is an lvalue: %p"                                        
        .text                                                                  
        .globl  example0                                                       
        .type   example0, @function                                            
example0:                                                                      
.LFB3:                                                                         
        .cfi_startproc                                                         
        push    rbp                                                            
        .cfi_def_cfa_offset 16                                                 
        .cfi_offset 6, -16                                                     
        mov     rbp, rsp                                                       
        .cfi_def_cfa_register 6                                                
        sub     rsp, 16                                                        
        mov     edi, 40                                                        
        call    malloc                                                         
        mov     QWORD PTR [rbp-8], rax                                         
        mov     rax, QWORD PTR [rbp-8]                                         
        mov     eax, DWORD PTR [rax]                                           
        mov     esi, eax                                                       
        mov     edi, OFFSET FLAT:.LC0                                          
        mov     eax, 0                                                         
        call    printf                                                         
        nop                                                                    
        leave                                                                  
        .cfi_def_cfa 7, 8                                                      
        ret                                                                    
        .cfi_endproc                                                           
.LFE3:                     
```
Over here, `*arr` is the `DWORD PTR [rax]`. Therefore I think lvalues must have the form `[<register/memory addr>]`.

### Restrict Qualified Pointers
Taken from https://en.cppreference.com/w/c/language/restrict, a type can be qualified with a subset of 'const', 'volatile' and 'restrict'. Restrict only applies to pointers to object types.
The 'restrict' qualifier is *just* a compiler hint indicating that all accesses (read/write) to the pointed object will only occur through this pointer. Usages of the pointer that do not obey this principle result in undefined behaviour.
Example cases where optimizations can be performed as a result of using a restrict qualified pointer:
```
void f(int n, int * restrict p, int * restrict q)
{
    while(n-- > 0)
        *p++ = *q++; // none of the objects modified through *p is the same
                     // as any of the objects read through *q
                     // compiler free to optimize, vectorize, page map, etc.
}
void g(void)
{
    extern int d[100];
    f(50, d + 50, d); // OK
    f(50, d + 1, d); // Undefined behavior: d[1] is accessed through both p and q in f
}
```

```
gcc has a __builtin_expect to signal that a if condition is more likely than others so instruction prefetching is not thrashed by a jump
```


### Modulus
The rule for modulus `a%b == z` is `b*x + z = a` so, `-10%7 == -3` as `7*-1 + (-3) == -10`. 
```
    int x = 1;
    int y = 7;
    printf("x `mod` y == %d, -x `mod` y == %d, x `mod` -y == %d, -x `mod` -y == %d\n", x%y, -x%y, x%-y, -x%-y);
    ---
    Output
    x `mod` y == 1, -x `mod` y == -1, x `mod` -y == 1, -x `mod` -y == -1
```


## PThread Interface
The type pthread_t is opaque - its an 8 byte identifier that is used internally. pthread_mutex_t and pthread_cond_t on ther other hand have the following properties:
```
(gdb) p lock
$5 = {__data = {__lock = 0, __count = 0, __owner = 0, __nusers = 5, __kind = 0, __spins = 0, __elision = 0, __list = {__prev = 0x0,
      __next = 0x0}}, __size = '\000' <repeats 12 times>, "\005", '\000' <repeats 26 times>, __align = 0}


(gdb) p cond
$6 = {__data = {__lock = 0, __futex = 5, __total_seq = 5, __wakeup_seq = 0, __woken_seq = 0, __mutex = 0x7fffffffe180,
    __nwaiters = 10, __broadcast_seq = 0},
  __size = "\000\000\000\000\005\000\000\000\005", '\000' <repeats 23 times>, "\200\341\377\377\377\177\000\000\n\000\000\000\000\000\000", __align = 21474836480}
```
On linux in glibc, both conditions and locks are implemented with futexes.
Looking at the kernel sources for normal mutexes: 
https://sourceware.org/git/?p=glibc.git;a=blob_plain;f=nptl/pthread_mutex_lock.c;hb=HEAD
simple:
      /* Normal mutex.  */
      LLL_MUTEX_LOCK (mutex);
      assert (mutex->__data.__owner == 0);

# define LLL_MUTEX_LOCK(mutex) \
  lll_lock ((mutex)->__data.__lock, PTHREAD_MUTEX_PSHARED (mutex))      

#define lll_lock(futex, private) __lll_lock (&(futex), private)

#define __lll_lock(futex, private)					      \
  ((void) ({								      \
    int *__futex = (futex);						      \
    if (__builtin_expect (atomic_compare_and_exchange_val_acq (__futex,       \
								1, 0), 0))    \
      {									      \
	if (__builtin_constant_p (private) && (private) == LLL_PRIVATE)	      \
	  __lll_lock_wait_private (__futex);				      \
	else								      \
	  __lll_lock_wait (__futex, private);				      \
      }									      \
  }))


# define atomic_compare_and_exchange_val_acq(mem, newval, oldval) \
  __atomic_val_bysize (__arch_compare_and_exchange_val,acq,		      \
		       mem, newval, oldval)


__lll_lock_wait (int *futex, int private)
{
  if (*futex == 2)
    lll_futex_wait (futex, 2, private); /* Wait if *futex == 2.  */
  while (atomic_exchange_acq (futex, 2) != 0)
    lll_futex_wait (futex, 2, private); /* Wait if *futex == 2.  */
}

https://chromium.googlesource.com/chromiumos/third_party/glibc-ports/+/factory-2368.B/sysdeps/unix/sysv/linux/arm/nptl/lowlevellock.h for the defintion of lll_lock(futex, private)


It has 3 states, 0 - unlocked, 1 - contended, 2 - locked

Common threading interface exposed in unix and is documented in the man pages.
### pthread_create
```
       int pthread_create(pthread_t *restrict thread,
              const pthread_attr_t *restrict attr,
              void *(*start_routine)(void*), void *restrict arg);
```
This creates and starts the thread calling start_routine with arg. 'thread' is populated with the ID.
On successful completion 0 is returned. The call can fail when there aren't enough resources/you don't have permission to perform the requested attributes or they are invalid.
Returning from 'start_routine' implicitly calls pthread_exit with the return value. (although the main function implicitly calls exit())

### pthread_cancel
       int pthread_cancel(pthread_t thread);
Runs the cancellation handlers, thread data destructors and then the thread is terminated. (not sure where these handlers are defined).

p_thread_exit should be called on main because it will block on the child threads (whereas otherwise with a call to exit all the children would be killed).
p_thread_join - waits for the target to exit, and if the value_ptr is non-null it gets the result of the called thread. This can return EDEADLK.

### mutes_init and destry
cannot reinitialize an undestroyed mutex, cannot destroy a locked mutex. (Get the EBUSY error).

### pthread_mutex_lock, trylock, unlock
There are different types of mutexes - default, error check, recursive, normal
Recursive ones are re-entrant. error check returns error values on error conditions - unlocking something unlocked, locking somethhing you own
Signals are attended to by processes waiting on mutex and then they go back to waiting.
Trylock returns an EBUSY and does not block if the mutex is locked.
The scheduling policy determines which thread gets access to the mutex when unlock is called and threads are waiting.


## Experiments
```

void* thread_action(void * null_arg)
{
  printf("Start!\n");
  sleep(2);
  printf("End!\n");
}


int main()
{
  pthread_t tid1;
  int res = pthread_create(&tid1, NULL, thread_action, NULL);
  printf("Main thread exits\n");
  pthread_exit(NULL);
}
////
[jcoutin@loydjcoutin1 concurrent]$ gcc test.c -lpthread
[jcoutin@loydjcoutin1 concurrent]$ ./a.out
Main thread exits
Start!
End!
```

Main waits on its child before exiting.

```
// when you remove the pthread_exit(NULL) it prints out Start!, Start! twice without fail

void* thread_action(void * null_arg)
{
  printf("Start!\n");
  sleep(2);
  printf("End!\n");
}


int main()
{
  pthread_t tid1;
  int res = pthread_create(&tid1, NULL, thread_action, NULL);
  printf("Main thread exits\n");
  //pthread_exit(NULL);
}

//////
[jcoutin@loydjcoutin1 concurrent]$ gcc test.c -lpthread
[jcoutin@loydjcoutin1 concurrent]$ ./a.out
Main thread exits
[jcoutin@loydjcoutin1 concurrent]$ ./a.out
Main thread exits
Start!
Start!
[jcoutin@loydjcoutin1 concurrent]$ ./a.out
Main thread exits
[jcoutin@loydjcoutin1 concurrent]$ ./a.out
Main thread exits
[jcoutin@loydjcoutin1 concurrent]$ ./a.out
Main thread exits
[jcoutin@loydjcoutin1 concurrent]$ ./a.out
Main thread exits
Start!
Start!

```

```

/*
what happens to a thread when the parent dies:
  parent is the main function - the child is killed - there can be unintended effects (for example I think the pc of the child thread got overwritten and it reverted to a previous instruction?)
  parent is not the main function (and we pthread_exit in the main function) - the child is not killed and is preserved.

  Does calling pthread_exit in the parent thread wait for the child thread? (not main)  - yes it does
  Does calling pthread_exit in the parent thread wait for the grand child thread? (not main) - yes all children in the thread group are waited for if you call pthread_exit somewhere in the hierarchy

  Inference, it's only the exit syscall that actually clears stuff up.

if a thread joins on a deceased thread does is still capture its value?
 */




void* grand_child_thread(void *null_arg)
{
  printf("Grand Child thread about to sleep.\n");
  sleep(2);
  printf("Grand Child about to exit.\n");
}
void* child_thread(void *null_arg)
{
  pthread_t grand_t;
  pthread_create(&grand_t, NULL, grand_child_thread, NULL);
  printf("Child Exiting!\n");
}

void* parent_thread(void * null_arg)
{
  pthread_t child_t;
  pthread_create(&child_t, NULL, child_thread, NULL);
  printf("Parent thread exiting!\n");
}


int main()
{
  pthread_t tid1;
  int res = pthread_create(&tid1, NULL, parent_thread, NULL);
  printf("Main thread pexits\n");
  pthread_exit(NULL);
}
```

```
// any thread can join join on any other thread.
void* branch1(void *return_val)                                                                                                        |[jcoutin@loydjcoutin1 concurrent]$ gcc -ggdb -pthread test.c
{                                                                                                                                      |[jcoutin@loydjcoutin1 concurrent]$
  printf("Branch1 sleeping for 5 seconds\n");                                                                                          |[jcoutin@loydjcoutin1 concurrent]$
  sleep(5);                                                                                                                            |[jcoutin@loydjcoutin1 concurrent]$
  *(int*)return_val = 10;                                                                                                              |[jcoutin@loydjcoutin1 concurrent]$ ./a.out
  printf("Branch1 exiting\n");                                                                                                         |Branch1 sleeping for 5 seconds
  return return_val;                                                                                                                   |Branch2 joining branch1
}                                                                                                                                      |a.out: test.c:84: main: Assertion `res == 10' failed.
                                                                                                                                       |Aborted
void* branch2(void *thread_to_wait)                                                                                                    |[jcoutin@loydjcoutin1 concurrent]$
{                                                                                                                                      |[jcoutin@loydjcoutin1 concurrent]$
  int *result;                                                                                                                         |[jcoutin@loydjcoutin1 concurrent]$ gcc -ggdb -pthread test.c
  pthread_t *branch1 = (pthread_t*)thread_to_wait;                                                                                     |[jcoutin@loydjcoutin1 concurrent]$ ./a.out
  printf("Branch2 joining branch1\n");                                                                                                 |Branch1 sleeping for 5 seconds
  pthread_join(*branch1, (void**)&result);                                                                                             |Branch2 joining branch1
  printf("Branch2 reports that the return value of branch1 is %d\n", *result);                                                         |Branch1 exiting
  assert(*result == 10);                                                                                                               |Branch2 reports that the return value of branch1 is 10
  return NULL;                                                                                                                         |[jcoutin@loydjcoutin1 concurrent]$
}                                                                                                                                      |
                                                                                                                                       |
int main()                                                                                                                             |
{                                                                                                                                      |
  pthread_t b1;                                                                                                                        |
  pthread_t b2;                                                                                                                        |
                                                                                                                                       |
  int res;                                                                                                                             |
                                                                                                                                       |
  pthread_create(&b1, NULL, branch1, (void*)&res);                                                                                     |
  pthread_create(&b2, NULL, branch2, &b1);                                                                                             |
                                                                                                                                       |
  pthread_exit(NULL);                                                                                                                  |
                                                                                                                                       |
}
```

### pthread_cond_wait

cond_wait takes a mutex and a cond_wait object. A pre-condition is that the calling thread must hold the lock on the mutex. The operation of cond_wait is to atomically release the mutex and sleep on the condition variable. The thread wakes up when a cond_signal/cond_broadcast has been sent. It then attempts to reacquire the mutex.
When the thread returns from pthread_cond_wait:
Throughout the lifetime of the pthread_cond_wait function, it will have held the lock, then blocked on cond, then woken up on a signal and then acquired the mutex.
The reason for the atomic release of the cond var and acquisition of the mutex is that if something wants to broadcast a signal to threads waiting on a signal you can guarentee that the broadcast signal will reach everyone who has started waiting on the signal.


## Question
 - Are the signals persistent? If a signal has been sent will something that subsequently wait on it be immediately woken up? - No, The pthread_cond_signal() and pthread_cond_broadcast() functions have no effect if there are no threads currently blocked on cond.
 - Can multiple threads block on the mutex? of course, otherwise there would be no reason for a broadcast.
 - Are the threads woken up in order?

conds are associated with a boolean predicate. Spurious wake ups may occur and the condition variable should always be checked before proceeding.
The reason that conds are always associated with a boolean predicate and not just for sending signals is that signals are lost. 

An example use of monitors might be to synchronize the starting of various threads so that they all begin together
```

const int NUM_THREADS = 5;
typedef struct monitor {
  pthread_cond_t *condition_var;
  pthread_mutex_t *lock;
} monitor;

typedef struct waiting_thread {
  monitor ready_monitor;
  int *thread_ready_count;
} waiting_thread;

void* waiter_function(void *args_)
{
  pthread_t thread_id = pthread_self();
  waiting_thread *args = (struct waiting_thread*)args_;
  int sleep_time = (int)(rand()/(float)RAND_MAX * 10);
  printf("TID %x: Sleeping for %d\n", thread_id, sleep_time);

  sleep(sleep_time);

  // guarentee
  // 1. The broadcast signal is only sent when all threads have woken up

  printf("TID %x: About to enter the mutex to increment thread ready count\n", thread_id);
  pthread_mutex_lock(args->ready_monitor.lock);
  *args->thread_ready_count += 1;
  if(*args->thread_ready_count == NUM_THREADS)
  {
    printf("TID %x: All %d have started, sending a broadcast signal\n", thread_id, *args->thread_ready_count);
    pthread_cond_broadcast(args->ready_monitor.condition_var);
  }
  else
  {
    printf("TID %x: Waiting for all threads to be ready, currently only %d have started\n", thread_id, *args->thread_ready_count);
    pthread_cond_wait(args->ready_monitor.condition_var, args->ready_monitor.lock);
    printf("TID %x: just received a signal, woken up and am plowing on\n", thread_id);
  };
  pthread_mutex_unlock(args->ready_monitor.lock);

}


void init_monitor(monitor *monitor)
{
  monitor->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  monitor->condition_var = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));

  pthread_cond_init(monitor->condition_var, NULL);
  pthread_mutex_init(monitor->lock, NULL);

}

int main()
{



  // test order of wakeups on condition signal
  int thread_ready_count = 0;

  monitor ready_monitor;
  init_monitor(&ready_monitor);

  struct waiting_thread ts;
  ts.ready_monitor = ready_monitor;
  ts.thread_ready_count = &thread_ready_count;

  pthread_t tids[NUM_THREADS];


  printf("MAIN: Spawning the threads\n");
  for(int i = 0; i < NUM_THREADS; i++)
  {
    pthread_t *t = tids + i;
    pthread_create(t, NULL, waiter_function, (void*)&ts);
  }
  printf("MAIN: Completed spawning %d threads\n", NUM_THREADS);
  pthread_mutex_lock(ts.ready_monitor.lock);
  while(*ts.thread_ready_count < NUM_THREADS)
  {
    printf("MAIN: Waiting for the all clear signal\n");
    pthread_cond_wait(ts.ready_monitor.condition_var, ts.ready_monitor.lock);
  }
  pthread_mutex_unlock(ts.ready_monitor.lock);

  printf("MAIN: Received the all clear, plowing ahead!\n");

  pthread_exit(NULL);
}
```


## The `time` function in linux
All experiments were done on:
```
processor       : 0
vendor_id       : GenuineIntel
cpu family      : 6
model           : 79
model name      : Intel(R) Xeon(R) CPU E5-2697 v4 @ 2.30GHz
stepping        : 1
microcode       : 0xb000025
cpu MHz         : 2294.686
cache size      : 46080 KB
physical id     : 0
siblings        : 1
core id         : 0
cpu cores       : 1
apicid          : 0
initial apicid  : 0
fpu             : yes
fpu_exception   : yes
cpuid level     : 13
wp              : yes

On linux version: 3.10.0-693.11.6.el7.x86_64 #1 SMP 
```

Time outputs something like this:
```
real    0m20.919s
user    0m11.118s
sys     0m15.768s
```

The `times` system call on linux reports the user and system time taken for the process and its children in clock ticks. clock ticks -> seconds is OS dependent and can be retrieved by sysconf(_SC_CLK_TCK). Although I found a definite disparity in their reporting:
```
[jcoutin@loydjcoutin1 experiment]$ ./a.out
User time: 0.000000 us, System Time: 0.000000 us
[jcoutin@loydjcoutin1 experiment]$ time ./a.out
User time: 0.000000 us, System Time: 0.000000 us

real    0m1.003s
user    0m0.002s
sys     0m0.001s
```
Thats the the wall clock time, time spent executing user instructions and time spent in the kernel for the process.
What is the time distribution for the following scenarios? (2 processors) [The code to test these sections is present inside `c_src/experiment/test_times.c`).

### When a process sleeps for 1 second
It registers as 0 from the perspective of times (I guess it literally hasn't taken enough of cpu time to really register in terms of clock ticks!)
### Looping through an array
Something weird:
```
  long large_sum = 0;
  for(int i = 0; i < 10e6; i++)
    {
      large_sum += i;
    }

  times(&tms);
  pprint_tms(&tms);
  /*
  $ time ./a.out
RAW: Clock Ticks - User time: 3, System Time: 0
User time: 30.000000 ms, System Time: 0.000000 ms

real    0m0.035s
user    0m0.034s
sys     0m0.002s

But changing the 10e6 to 1000000 gives me 0 on the user time and system time:
Relevant assembly before change: (its cut 10e6 is actually 10^7) 
.L30:
        mov     eax, DWORD PTR [rbp-12]
        cdqe
        add     QWORD PTR [rbp-8], rax
        add     DWORD PTR [rbp-12], 1
.L29:
        cvtsi2sd        xmm0, DWORD PTR [rbp-12]
        movsd   xmm1, QWORD PTR .LC12[rip]
        ucomisd xmm1, xmm0
        ja      .L30


Relevant assembly after change:
.L30:
        mov     eax, DWORD PTR [rbp-12]
        cdqe
        add     QWORD PTR [rbp-8], rax
        add     DWORD PTR [rbp-12], 1
.L29:
        cmp     DWORD PTR [rbp-12], 999999
        jle     .L30

*/
```

Also A tick is an arbitrary unit for measuring internal system time. There is usually an OS-internal counter for ticks; the current time and date used by various functions of the OS are derived from that counter.
The tick on the machine that I'm using (Linux 3.10 x64 is 1/100th of a second - so it only has accuracy upto a 10 milliseconds).

### Adding a number:
```
  long large_sum = 0;
  for(long i = 0; i < 1000000000; i++)
    {
      large_sum += i;
    }

///
.L30:
        mov     rax, QWORD PTR [rbp-16]
        add     QWORD PTR [rbp-8], rax
        add     QWORD PTR [rbp-16], 1
.L29:
        cmp     QWORD PTR [rbp-16], 9999999
        jle     .L30
```
User time: 2610.000000 ms, System Time: 0.000000 ms
User time: 2630.000000 ms, System Time: 0.000000 ms
User time: 2610.000000 ms, System Time: 0.000000 ms
User time: 2600.000000 ms, System Time: 0.000000 ms
User time: 2610.000000 ms, System Time: 0.000000 ms
User time: 2590.000000 ms, System Time: 0.000000 ms
User time: 2620.000000 ms, System Time: 0.000000 ms
User time: 2610.000000 ms, System Time: 0.000000 ms
User time: 2600.000000 ms, System Time: 0.000000 ms
User time: 2590.000000 ms, System Time: 0.000000 ms
User time: 2630.000000 ms, System Time: 0.000000 ms
Mean: 2609, std dev: 13

So (load, add, add, cmp, jmp) cycle takes 2609/10^9 seconds which is 3microseconds.

Adding in an additional increment to a variable I get:
User time: 2720.000000 ms, System Time: 0.000000 ms
User time: 2810.000000 ms, System Time: 0.000000 ms
User time: 3030.000000 ms, System Time: 0.000000 ms
User time: 2730.000000 ms, System Time: 0.000000 ms
User time: 2810.000000 ms, System Time: 0.000000 ms
User time: 2800.000000 ms, System Time: 0.000000 ms
User time: 2690.000000 ms, System Time: 0.000000 ms
User time: 2810.000000 ms, System Time: 0.000000 ms
User time: 2720.000000 ms, System Time: 0.000000 ms
User time: 2720.000000 ms, System Time: 0.000000 ms
User time: 2790.000000 ms, System Time: 0.000000 ms
User time: 2780.000000 ms, System Time: 0.000000 ms
User time: 2810.000000 ms, System Time: 0.000000 ms
User time: 2750.000000 ms, System Time: 0.000000 ms
User time: 2810.000000 ms, System Time: 10.000000 ms
User time: 2780.000000 ms, System Time: 0.000000 ms
User time: 2740.000000 ms, System Time: 0.000000 ms
User time: 2750.000000 ms, System Time: 0.000000 ms
User time: 2740.000000 ms, System Time: 0.000000 ms
User time: 2800.000000 ms, System Time: 0.000000 ms
Mean: 2779, Std Dev. 70

170ms difference which means that an invidual add instruction takes approximately 17 nanoseconds.
(Using (-) instead, there's no difference).
User time: 2690.000000 ms, System Time: 0.000000 ms
User time: 2720.000000 ms, System Time: 0.000000 ms
User time: 2770.000000 ms, System Time: 0.000000 ms
User time: 2780.000000 ms, System Time: 0.000000 ms
User time: 2690.000000 ms, System Time: 0.000000 ms
User time: 2820.000000 ms, System Time: 0.000000 ms
User time: 2700.000000 ms, System Time: 0.000000 ms
User time: 2830.000000 ms, System Time: 0.000000 ms
User time: 2820.000000 ms, System Time: 0.000000 ms
User time: 2780.000000 ms, System Time: 0.000000 ms
User time: 2700.000000 ms, System Time: 0.000000 ms
User time: 2830.000000 ms, System Time: 0.000000 ms
User time: 2840.000000 ms, System Time: 0.000000 ms
User time: 2790.000000 ms, System Time: 0.000000 ms
User time: 2750.000000 ms, System Time: 0.000000 ms
User time: 2780.000000 ms, System Time: 0.000000 ms
User time: 2860.000000 ms, System Time: 0.000000 ms
User time: 2750.000000 ms, System Time: 0.000000 ms
User time: 2780.000000 ms, System Time: 0.000000 ms
User time: 2820.000000 ms, System Time: 0.000000 ms
Mean 2775, Std Dev: 53

Creating a thread and noop does nothing (just pthread_exits)
```
  for(long i = 0; i < 100000; i++)
    {
      large_sum += i;
      pthread_create(&t, NULL, noop, &val);
    }
```
Without the pthread_creation:
real    0m0.003s
user    0m0.001s
sys     0m0.002s
With the pthread_creation and a child that does nothing:
real    0m3.554s
user    0m0.467s
sys     0m3.449s
With the pthread_creation and a child that sums numbers from 1 - 10^7:
real    7m20.416s
user    14m17.900s
sys     0m3.828s
With the pthread_creation and a child that sums numbers from 1 - 10^6:
real    0m47.152s
user    1m26.517s
sys     0m3.791s
Sums numbers from 1 - 10^5
real    0m8.715s
user    0m8.925s
sys     0m3.509s
Sums from 1 - 10^4
real    0m4.063s
user    0m1.336s
sys     0m3.570s


A thread takes 3ms to sum 10^6 numbers.
So, thread creation takes 4 microseconds of user time and 30 microseconds of system time based on the above.
Looking at the last table it's nice to see that system time does not change - since the threads dont actually do any system related work.
It's also nice to see that the user time grows by roughly a factor of 10 every time we multiply the number of ops a user thread is doing.
With the 10^4 loops:
10^6 ops - 3ms (I think the difference from before lies in the fact that the value must be in the cache - wasn't able to verify this by changing the update variable to volatile).
1 op - 3ns
10^4 ops = 30 us (micro) per thread, 10^5 threads, should be 3s total user time, just doing actual ops.
Then there's 450ms spent in thread creation which gives 3450ms of total user time. With two processors working perfectly that gives 1.725ms of total user time. However, the reporting suggests that it actually takes < 1.4ms. Which means that processing a loop actually takes around 9.5 microseconds.




Throughput:


How much time does a thread context switch take?
Number of threads versus throughput.
How does blocking IO work with threads?
 - Does a blocked thread get swapped in for a live one?



### What about simple things like printing.
    - How does the time change with the size of the string.
### Threads
 - Creating a thread
 - Joining a thread
### Mutexes
 - Creating a mutex
 - Taking a lock on a mutex with no contention
 - Taking a lock on a mutex with contention

How do the following distributions change when we move to a single processor? (you can use `taskset` to manipulate the 'cpu affinity' of existing and new processes)
### When a process sleeps for 1 second
### What about simple things like printing.
    - How does the time change with the size of the string.
### Threads
 - Creating a thread
 - Joining a thread
### Mutexes
 - Creating a mutex
 - Taking a lock on a mutex with no contention
 - Taking a lock on a mutex with contention



## Atomicity of updates to read:

All of the tests were tried 10000 times.
So splitting the read and write into two instructions gives a 93.42 chance of success with 10 threads and not splitting it has the same effect. 
```
/*
Splitting up the read and write
Total: 10000, Failed: 658, Correct: 9342, Percent Success: 93.420000

real    0m5.818s
user    0m0.883s
sys     0m5.416s
*/
void* incers(void *null)
{
  for(int i = 0; i < INC_COUNT; i++)
  {
    int read = protected;
    protected = read + 1;
  }

}


// Changing the update loop to
/*
Total: 10000, Failed: 326, Correct: 9674, Percent Success: 96.740000

real    0m5.754s
user    0m0.759s
sys     0m5.402s
*/
void* incers(void *null)
{
  for(int i = 0; i < INC_COUNT; i++)
  {
    //lock(&global_lock);
    //usleep(rand()*100);
    protected += 1;
    //unlock(&global_lock);
  }

}

// which compiles to: (changing to ++protected, makes no difference)
.L24:
        mov     eax, DWORD PTR protected[rip]
        add     eax, 1
        mov     DWORD PTR protected[rip], eax
        add     DWORD PTR [rbp-4], 1
.L23:
        mov     eax, 1000
        cmp     DWORD PTR [rbp-4], eax
        jl      .L24
(the 



// With a proper lock
void* incers(void *null)
{
  for(int i = 0; i < INC_COUNT; i++)
  {
    //lock(&global_lock);
    pthread_mutex_lock(&global);
    int read = protected;
    //usleep(rand()*100);
    protected = read + 1;
    pthread_mutex_unlock(&global);
    //unlock(&global_lock);
  }

}


Total: 10000, Failed: 0, Correct: 10000, Percent Success: 100.000000

real    0m7.599s
user    0m3.279s
sys     0m6.202s


```

Full code here:
```


int NUM_THREADS = 10;
int INC_COUNT = 1000;

void* incers(void *null)
{
  for(int i = 0; i < INC_COUNT; i++)
  {
    int read = protected;
    protected = read + 1;
  }

}


typedef struct stats {
  int correct;
  int total;
} stats;

void print_stats(stats st)
{
  printf("Total: %d, Failed: %d, Correct: %d, Percent Success: %f\n", st.total, st.total - st.correct, st.correct, 100.0*st.correct/(float)st.total);

}

int main()
{
  stats st = { .correct = 0, .total = 0 };


  for(int x = 0; x < 10000; x++){
    global_lock.state = 0;

    pthread_t ts[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++) pthread_create(ts + i, NULL, incers, NULL);

    for(int i = 0; i < NUM_THREADS; i++) pthread_join(ts[i], NULL);

    printf("Checking");
    if(protected != NUM_THREADS * INC_COUNT) {
      printf("Iteration: %d, Check Failed!: protected (%d) != NUM_THREADS*INC_COUNT (%d)\n", x, protected, NUM_THREADS * INC_COUNT);
    }
    else st.correct++;
    st.total++;
  }

  print_stats(st);

  pthread_exit(NULL);

}

```

When changing to compile with -O3, the loop is optimized and reduced to an add instruction (I'm not sure what the rip is doing in address but protected is a label). (Interestingly it doesn't use an `inc` instruction even if I remove the loop around it). It also does away with the saving of the base pointer and saving of the args on the stack.
```
add protected[rip], 1000`
incers:
.LFB13:
        .loc 1 82 0
        .cfi_startproc
.LVL17:
.LBB12:
        .loc 1 87 0
        add     DWORD PTR protected[rip], 1
.LVL18:
.LBE12:
        .loc 1 92 0
        ret
        .cfi_endproc
```

Running for 10000 iterations, with 200 threads:
Total: 10000, Failed: 446, Correct: 9554, Percent Success: 95.540000

So even an add instruction is not atomic across multiple processors.

When actually running this with `taskset --cpu-list 1 ./a.out` I get no test failures. So `add` is atomic when you have a single processor.
```
$ taskset --cpu-list 1 ./a.out
Total: 1000, Failed: 0, Correct: 1000, Percent Success: 100.000000

real    0m16.085s
user    0m1.152s
sys     0m11.765s


$ time taskset --cpu-list 0,1 ./a.out
Total: 1000, Failed: 49, Correct: 951, Percent Success: 95.100000

real    0m13.067s
user    0m1.270s
sys     0m12.994s

```
Looks like a massive amount of time is also spent creating the threads. 1000*200 threads were created here (around 60 microseconds to create a new thread?).

## Implementing my own locks:
For a basic CAS:
```
bool compare_and_swap(lock_t *lock, int a, int b)                               
{                                                                               
  pthread_mutex_lock(&cas_lock);                                                
  if(lock->state != a) {                                                        
    pthread_mutex_unlock(&cas_lock);                                            
    return 0;                                                                   
  }                                                                             
  lock->state = b;                                                              
                                                                                
  pthread_mutex_unlock(&cas_lock);                                              
                                                                                
  return 1;                                                                     
                                                                                
}
                                                                                
//The lock function:
  while(1)                                                                      
  {                                                                             
    if(lock->state == 0)                                                        
    {                                                                           
      compare_and_swap(lock, 0, 1);                                             
    }                                                                           
    else if(lock->state == 1)                                                   
    {                                                                           
      bool result = compare_and_swap(lock, 1, 2);                               
      if(result) break;                                                         
    }                                                                           
    else sleep(SLEEP_INTERVAL);                                                 
  }   

// Unlock just sets the lock->state = 0

$ time ./a.out
Starting
Completed test.Total: 1000, Failed: 0, Correct: 1000, Percent Success: 100.000000

real    0m20.919s
user    0m11.118s
sys     0m15.768s


```

### How strong a CAS do you really need to implement a lock?



### Evaluating concurrent algorithms:

Throughput - Number of non-locking operations
Lock contention:
 - Number of times a thread sleeps





# Lockless programming
https://docs.microsoft.com/en-us/windows/desktop/DxTechArts/lockless-programming

Another good example: https://preshing.com/20120612/an-introduction-to-lock-free-programming/

Issues to deal with, non-atomic operations and re-ordering of instructions.
'inc' operations are non-atomic for a multi-core system but are atomic on a single core system. (read, write incremented value). Reads are not usually re-orderderd relative to other reads and the same for writes. Reads may be re-ordered relative to other writes.
The main constructs used to prevent reordering of reads and writes are called read-acquire and write-release barriers. A read-acquire is a read of a flag or other variable to gain ownership of a resource, coupled with a barrier against reordering. Similarly, a write-release is a write of a flag or other variable to give away ownership of a resource, coupled with a barrier against reordering.
The formal definitions, courtesy of Herb Sutter, are:

 - A read-acquire executes before all reads and writes by the same thread that follow it in program order.
 - A write-release executes after all reads and writes by the same thread that precede it in program order.

If a barrier is not used an example where data-sharing may fail is the following:
A read to some part of the resource for a read acquire moves ahead of the release from a write release.

Note, all this synchronization only makes sense in the context of cacheable memory. Non cacheable memory is not affected by instruction reordering. The volatile keyword is used to guarentee that the memory location is read/written to directly from memory is not cached. This doesn't mitigate instruction reordering though.


Example of a possible case for live-lock:
```
while (X == 0)
{
    X = 1 - X;
}
```
If t1 and t2 constently perform the update right after each other they will never break out of the loop.

Different compiler/processor optimizations: caching, instruction pre-fecthing, branch prediction, instruction re-ordering.

# TODO
 - One of the issues of using these refs to pass pointers is that you no longer have types which sucks. Maybe create a macro that takes the name of the ref, the name of the value and the type of the value and casts creates a new ref, gets the value and casts the ref to the value. Alternatively for the longer route - dont think you really need this - I think there is a way to automatically generate definitions and the corresponding bindings functions for a ref of a new type using macros but haven't investigated yet. For e.g., you'd hava ref_int, ref_hash_map etc.

 - Improve the implementation of hash maps w.r.t the rebalancing and calculation of rebalancing by using bit shift operators.
 - We track the local bindings in a hashmap which we pass around (as a pointer) instead of pushing them onto the stack (as is suggested in the paper). The garbage collector would use the layout in the info table to find out about the state of the heap. One (in retrospect stupid reason) was that I wanted to declare a case_frame struct (and you can't fit a variable sized list of free variables into a struct)
 - The pointer table keeps growing - is there a way to shrink it? (considering other stuff holds references into it you can't really move elements around. Check if it's possible to free interior portions of a malloced structure.
 - Todo, create an abstraction in front of our pointer table that allows us to interact with the pointers better - I think I'm actually implementing references here.
 - There is no need to have local bindings - just index straight into the pointer table.
 - It seems like C doesn't perform tail call optimization

# Makefiles
Variables are specified as <key> := <value>. The convention is to use capitals for keys.
You refer to variables with $(<key>)

The usualy layout is to have an all target and a clean target at least. clean is marked as .PHONY to note that there is nothing generated. The generated files go into a target directory whose internal dir structure matches the source dir structure.

An example rule to compile a .o files from the corresponding .c file. The % denote variables that are captured in rule local variables.
The targets are captured in $@ and the dependency in $^. There are more variables and funnier ways of using them.

```
test/stg/target/stg/plus_int/%.o: src/stg/plus_int/%.c
	mkdir -p test/stg/target/stg/plus_int
	$(GCC_CMD) $^ -c -o $@
```


