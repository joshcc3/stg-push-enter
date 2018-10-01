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



# C Functions:
When a case is being evaluated and the only options are case constructors:
The arguments can only ever evaluate to thunks or the case constructor (not func/pap)

# Memory Representation of Heap Objects
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



# Code flow
## Examples
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

## Notes
 - The case continuation updates a particular binding it knows are not among the cases free variables so that the actual continuation can pluck the referenced object out at that binding.

 - Single argument functions don't need to perform an argument satisfaction check - the function call rules would never apply for that to be the case.

 - Another optimization that we follow from the paper is to limit the number of su saves. As the paper describes, the only time we care about saving su is when not saving it would mean that the scrutinee of the case or update frame would get its argument satisfaction check wrong (i.e. the scrutinee evaluates to a partial function that mistakes the number of arguments available because we haven't saved the closes stack frame).
 Thus we only care about this when the scrutinee evaluates to a function.
 Since it's rare for the scrutinee of a case expression to evaluate to a function we never save su for stack frames. In the odd case (like for `seq`) we push an update frame that updates su on top of the case frame.

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