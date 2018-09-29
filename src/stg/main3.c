#include "plus_int/statics.h"
#include "bindings.h"

// TODO, instead of returning functions should tail call the continuation at the top of the stack
// This avoids the entire stack trace building up like we do.

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



ref inc_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);

     struct hash_map *bindings = (struct hash_map*)thunk[1];

     struct info_table* pap_info = (struct info_table*)new(sizeof(struct info_table));
     pap_info->type = 4;
     pap_info->extra.pap = { .info_ptr = &plus_info_table, .size = 1 };
     pap_[0] = pap_info;
     ref one_ref;
     get_binding(bindings, 0, &one_ref)
     *(ref*)pap_  = one_ref;
     return pap_ref;
}


ref list_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

    ref list_ref;
    new_ref(sizeof(void*) + sizeof(ref)*2, &list_ref);

    ref one;
    get_binding(bindings, 0, &one);

    ref inced;
    get_binding(bindings, 2, &inced);

    void **list = (void**)get_ref(list_ref);
    list[0] = &cons_info_table;
    list[1] = one;
    list[2] = inced;

    return list_ref;
}

ref tail1_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref inced;
     get_binding(bindings, 2, &inced);

     return tail(inced);
}

ref element2_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref tail;
     get_binding(bindings, 5, &tail);

     return head(tail);
}

ref element1_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref inced;
     get_binding(bindings, 2, &inced);

     return head_fast_call(inced);
}

ref inced_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref inc;
     ref list;

     get_binding(bindings, 1, &inc);
     get_binding(bindings, 3, &list);

     return map_fast_call(inc, list);
}



ref main_function(ref)
{
    ref one_ref;
    new_ref(sizeof(void*) + sizeof(int), &one);
    void **one = (void**)get_ref(one_ref);
    one[0] = &int_constructor_info_table;
    *(int*)(one[1]) = 1

    put_binding(bindings, 0, one);

    ref inc = create_thunk(bindings, inc_cont);
    put_binding(bindings, 1, inc);

    ref inced = create_thunk(bindings, inced_cont);
    put_binding(bindings, 2, inced);

    ref list = create_thunk(bindings, list_cont);
    put_binding(bindings, 3, list);

    ref element1 = create_thunk(bindings, element1_cont);
    put_binding(bindings, 4, element1);

    ref tail1 = create_thunk(bindings, tail1_cont);
    put_binding(bindings, 5, tail1);

    ref list = create_thunk(bindings, tail1_cont);
    put_binding(bindings, 6, list);

    ref element2 = create_thunk(bindings, element2_cont);
    put_binding(bindings, 7, element2);

    push_case_frame(case_cont, 8, bindings);
    return case_continuation(plus_int_fast(element1, element2));
}