

/*
main = let one = I# 1
       let inc = THUNK (plus_int one)
       let inced = THUNK (map inc list)
       let list = THUNK (Cons one inced)
       let element1 = THUNK (head inced)
       let tail1 = THUNK (tail inced)
       let element2 = THUNK (head tail1)
       in case plus_int element1 element2 of
          I# z -> print_int z

*/


ref main_function(ref)
{
    create I# 1
    bind one to it
    create thunk plus_int one
    bind inc to it
    create thunk map inc list
    bind inced to it
    create a thunk of Cons one inced
    bind list to it
    create thunk of head inced
    bind element1 to it
    create a thunk of tail inced
    bind tail1 to it
    create a thunk of head tail1
    bind element2 to it
    push a case frame
    push element2
    push element1
    pop case frame(fast call plus_int)
}