#include "maybe.h"

struct maybe_info_table

struct nothing {
    struct info_table *;
};

struct just {
    struct info_table *;
    struct ref value;
};

struct ref case_continuation(struct ref bindings)
{
    struct con c = (struct info_table*)maybe_val->extra.constructor;
    if(c.con_num == 0)
    {
        assert(c.arity == 1);
        bind value to x
        return just_case(bindings);
    }
    else
    {
        assert(c.con_num == 1);
        return nothing;
    }
}

struct ref func_continuation(struct ref){
    inspect the func
    if its a thunk:
      push the case frame(func_continuation)

    else it must be a value
       if its a pap
         unpack the pap onto the stack
         push x
         enter the slow_entry_point of the function
       then..
       must be a func:
         push x
         enter the slow_entry_point of the function.
       // cannot be a constructor
}

/*
heap objects:
 FUNC(x1 x2 .. xn -> e) |
 PAP(f x1 x2 .. xn) |
 BLACKHOLE |
 THUNK(e) |
 CONC( C x1 x2 .. xn)

*/
/*
  Fast entry point.
  maybe val func default = case val of
                               Just x -> func x
                               Nothing -> default
*/
struct ref maybe(struct ref maybe_ref, struct ref just_case, struct ref nothing)
{
    struct hash_map *bindings;
    init_hash_map(&bindings, 6, );

    void **maybe_value = (void**)get_ref(maybe_ref);

    if(constructor)
    {
        bind x -> value
        push x
        func continuation
    }
    else
    {
        assert(thunk);
        push the case frame (case_continuation)
        push update frame
        set the ref to a blackhole
        enter the thunk
        pop the update continuation
        pop the case continuation
    }
}