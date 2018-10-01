#include "stg/plus_int/static.h"
#include "bindings.h"
#include "containers/mmanager.h"
#include "stg/heap.h"
#include "stg/list/list.h"
#include <stdio.h>

// TODO each constructor should expose functions to create an instance of the constructor

// TODO, instead of returning functions should tail call the continuation at the top of the stack
// This avoids the entire stack trace building up like we do.


ref inc_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     NEW_REF(pap_ref, void**, sizeof(void*) + sizeof(ref), pap)

     NEW(struct info_table, pap_info)
     pap_info->type = 4;
     struct pap pap_info_extra = { .info_ptr = &plus_info_table, .size = 1 };
     pap_info->extra.pap_info = pap_info_extra;

     pap[0] = (void*)pap_info;

     ref one_ref;
     get_binding(bindings, 0, &one_ref);

     *(ref*)(pap + 1) = one_ref;

     return pap_ref;
}

ref tail1_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref inced;
     get_binding(bindings, 2, &inced);

     return tail_fast(inced);
}

ref element2_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref tail;
     get_binding(bindings, 5, &tail);

     return head_fast(tail);
}

ref element1_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref inced;
     get_binding(bindings, 2, &inced);

     return head_fast(inced);
}

ref inced_cont(ref thunk_ref)
{
     void** thunk = (void**)get_ref(thunk_ref);
     struct hash_map *bindings = (struct hash_map*)thunk[1];

     ref inc;
     ref list;

     get_binding(bindings, 1, &inc);
     get_binding(bindings, 3, &list);

     return map_fast(inc, list);
}

ref case_cont(hash_map *bindings)
{
  ref i_ref;
  get_binding(bindings, 8, &i_ref);
  void** i_ = (void**)get_ref(i_ref);
  if((*(struct info_table **)i_)->type == 1)
  {
    printf("%d\n", *(int*)(i_ + 1));
    ref null;
    return null;
  }

  else return thunk_continuation(i_ref, case_cont, bindings, 8, i_ref);

  return i_ref;
}

/*
main = let one = I# 1
       let inc = THUNK (plus_int one)
       let inced = THUNK (map inc list)
       let list = Cons one inced
       let element1 = THUNK (head inced)
       let tail1 = THUNK (tail inced)
       let element2 = THUNK (head tail1)
       in case plus_int element1 element2 of
          I# z -> print_int z
*/
ref main_function(ref null)
{
    ref one;
    c_int(1, &one);

    hash_map *bindings;
    init_bindings(&bindings);

    put_binding(bindings, 0, one);

    ref inc = create_thunk(bindings, inc_cont);
    put_binding(bindings, 1, inc);

    ref inced = create_thunk(bindings, inced_cont);
    put_binding(bindings, 2, inced);

    NEW_REF(list_ref, Cons*, sizeof(Cons), list)
    c_cons(one, inced, list);
    put_binding(bindings, 3, list_ref);

    ref element1 = create_thunk(bindings, element1_cont);
    put_binding(bindings, 4, element1);

    ref tail1 = create_thunk(bindings, tail1_cont);
    put_binding(bindings, 5, tail1);

    ref element2 = create_thunk(bindings, element2_cont);
    put_binding(bindings, 7, element2);

    push_case_frame(case_cont, 8, bindings);

    return case_continuation(plus_int_fast(element1, element2));
}