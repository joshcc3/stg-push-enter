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

#Objectives
 - Manually 'compile' stg programs into C
 - Write a parallel generational garbage collector for the runtime based on ()

# Why do we need continuations?
They're kind of like return addresses with packed info. They are needed because the same sections of code are visited from different points.  (like after a case or after a thunk)

# Memory Representation of Heap Objects
Most memory representation definitions are inside static.h. All heap objects consist of an info table pointer followed by a payload, the descriptions of which are given in the paper.
Constructor: (<info table pointer>, <arg1> ... ) where the args can be either boxed or unboxed values. The layout is specified in the info table.
Thunk: (<info table pointer>, <hash map pointer>) the hash map points to the free variables inside the body of the thunk
Fun(<info table pointer>, arity) number of arguments this takes?
PAP(<info table pointer to function>, arity?)
BLACKHOLE: (<info ptr to thunk>,)

The stack pointer starts of at the byte just after the highest point in the stack.

Remember to bind the actual value of the case when the case is evaluated.


TODO: Find a more elegant way of handling the binding of the case continuation result.


In order to perform the 'Update' continuation we cannot just pass around pointers to the heap object.
This is because when attempting to update a heap object you'll have to go through all the bindings maps (that map a bound variable to the heap object  - represented as maps from int -> ptr)) and update the reference to the heap object in them.
One alternative is to maintain a pointer table and pass around pointers into the pointer table and update entries in the pointer table when updating through an Update continuation.
Or you could maintain a global bindings map - then the key of the map would have to be globally unique and you would pass around the keys to the object. This is basically the same as the pointer table approach with some added overhead.
My decision was to maintain a pointer table and have the local global bindings point to the pointer table references referred to in the pointer table. Requesting a new reference adds an entry into the pointer table. Adding a mapping from to a hashmap
maps the key to the reference in the table.
Using pointers internal to a structure is a terrible idea (in this case on resize all your pointers would be invalidated) so you store them as offsets from the start of the index.
One tricky question is how to maintain the pointer table.. evidently its append only and only the garbage collector can really "compact it". 





# Notes about C
`malloc` is contained in `stdlib.h`.



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
        .section        .rodata                                                                                       │
.LC0:                                                                                                                 │
        .string "This is an lvalue: %p"                                                                               │
        .text                                                                                                         │
        .globl  example0                                                                                              │
        .type   example0, @function                                                                                   │
example0:                                                                                                             │
.LFB3:                                                                                                                │
        .cfi_startproc                                                                                                ├────────────────────────────────────────────────────────────
        push    rbp                                                                                                   │ubuntu@ip-172-31-10-151:~/projects/stg-push-enter$ 
        .cfi_def_cfa_offset 16                                                                                        │
        .cfi_offset 6, -16                                                                                            │
        mov     rbp, rsp                                                                                              │
        .cfi_def_cfa_register 6                                                                                       │
        sub     rsp, 16                                                                                               │
        mov     edi, 40                                                                                               │
        call    malloc                                                                                                │
        mov     QWORD PTR [rbp-8], rax                                                                                │
        mov     rax, QWORD PTR [rbp-8]                                                                                │
        mov     eax, DWORD PTR [rax]                                                                                  │
        mov     esi, eax                                                                                              │
        mov     edi, OFFSET FLAT:.LC0                                                                                 │
        mov     eax, 0                                                                                                │
        call    printf                                                                                                │
        nop                                                                                                           │
        leave                                                                                                         │
        .cfi_def_cfa 7, 8                                                                                             │
        ret                                                                                                           │
        .cfi_endproc                                                                                                  │
.LFE3:                     
```
Over here, `*arr` is the `DWORD PTR [rax]`. Therefore I think lvalues must have the form `[<register/memory addr>]`.


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

# TODO
 - Improve the implementation of hash maps w.r.t the rebalancing and calculation of rebalancing by using bit shift operators.
 - We track the local bindings in a hashmap which we pass around (as a pointer) instead of pushing them onto the stack (as is suggested in the paper). The garbage collector would use the layout in the info table to find out about the state of the heap. One (in retrospect stupid reason) was that I wanted to declare a case_frame struct (and you can't fit a variable sized list of free variables into a struct)
 - The pointer table keeps growing - is there a way to shrink it? (considering other stuff holds references into it you can't really move elements around. Check if it's possible to free interior portions of a malloced structure.
 - Todo, create an abstraction in front of our pointer table that allows us to interact with the pointers better - I think I'm actually implementing references here.
 - There is no need to have local bindings - just index straight into the pointer table.
# TODO
You create a file that contains the main functino that gets linked against the other stuff 
