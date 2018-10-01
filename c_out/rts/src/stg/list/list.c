#include <assert.h>

#include "list.h"
#include "containers/hash_map.h"
#include "containers/mmanager.h"
#include "stg/bindings.h"
#include "stg/plus_int/static.h"
#include "typeclasses.h"
#include "stg/heap.h"

void init_list()
{
    cons_info_table = (struct info_table){ .type = 1, .extra = { .constructor = { .arity = 2, .con_num = 0 } } };

    nil_info_table = (struct info_table){ .type = 1, .extra = { .constructor = { .arity = 0, .con_num = 1 } } };

    arg_entry map_entries[2];
    map_entries[0] = (struct arg_entry) { .pointer = true, .offset = 0 };
    map_entries[1] = (struct arg_entry) { .pointer = true, .offset = sizeof(ref) };
    map_info_table = (struct info_table) { .type = 0,
                       .extra.function = { .arity = 2, .slow_entry_point = map_slow },
                       .layout = { .num = 2, .entries = map_entries }
                      };

    new_ref(sizeof(Nil), &nil_value);
    ((Nil*)get_ref(nil_value))->info_ptr = &nil_info_table;
}

void c_cons(ref value, ref next, Cons* res)
{
    struct info_table *tbl = *(struct info_table**)get_ref(next);
    assert(tbl->type == 5 || tbl->type == 1);

    res->info_ptr = &cons_info_table;
    res->value = value;
    res->next = next;
}

ref map_thunk1(ref thunk)
{
      hash_map* bindings = THUNK_GET_BINDINGS(thunk);

      GET_BINDING(f_ref, void**, f, 0, bindings)

      ref x_ref;
      get_binding(bindings, 2, &x_ref);

      struct info_table f_info = **(struct info_table**)f;
      if(f_info.type == 5)
      {
        push_update_frame(f_ref);
        update_continuation(f_info.extra.thunk_info.return_address(f_ref));
        // TODO: should really use a loop over here instead
        return map_thunk1(thunk);
      }
      else if(f_info.type == 4)
      {
        push_ptr(x_ref);
        unroll_pap(f);

        ref null;
        return f_info.extra.pap_info.info_ptr->extra.function.slow_entry_point(null);
      }
      else if(f_info.type == 0)
      {
        push_ptr(x_ref);
        ref null;
        return f_info.extra.function.slow_entry_point(null);
      }
      else assert(false);
}

ref map_thunk2(ref thunk)
{
    hash_map* bindings = THUNK_GET_BINDINGS(thunk);
    ref a1;
    ref a2;
    get_binding(bindings, 0, &a1);
    get_binding(bindings, 3, &a2);
    return map_fast(a1, a2);
}

/*
map f l = case l of
            Cons x xs -> let x' = THUNK (f x) in
                         let f' = THUNK (map f xs) in
                         let c' = Cons x' f'
                         in c'
            Nil -> Nil
*/
ref map_case_cont(struct hash_map* bindings)
{
    ref func_ref;
    get_binding(bindings, 0, &func_ref);

    GET_BINDING(list_ref, void**, l_, 1, bindings);

    struct info_table *l_info = (struct info_table*)(*l_);
    if(l_info->type == 1)
    {
        if(l_info->extra.constructor.con_num == 0)
        {
            Cons *l = (Cons*)l_;
            put_binding(bindings, 2, l->value);
            put_binding(bindings, 3, l->next);
            ref thunk1_ref = create_thunk(bindings, map_thunk1);
            put_binding(bindings, 4, thunk1_ref);

            ref thunk2_ref = create_thunk(bindings, map_thunk2);
            put_binding(bindings, 5, thunk2_ref);

            ref cons_ref;
            new_ref(sizeof(Cons), &cons_ref);
            Cons *cons = (Cons *)get_ref(cons_ref);
            cons->info_ptr = &cons_info_table;
            cons->value = thunk1_ref;
            cons->next = thunk2_ref;

            return cons_ref;
         }
         else
         {
            assert(l_info->extra.constructor.con_num == 1);
            return nil_value;
         }
    }
    else return thunk_continuation(list_ref, map_case_cont, bindings, 1, list_ref);

}

// TODO: intialize the layout for every function info table, (plus_int, map)

ref map_fast(ref function, ref list)
{
    struct hash_map *bindings;
    init_bindings(&bindings);

    put_binding(bindings, 0, function);
    put_binding(bindings, 1, list);
    return map_case_cont(bindings);
}

ref map_slow(ref null)
{
  if(arg_satisfaction_check(sizeof(ref)*2))
  {
    ref arg1;
    ref arg2;
    pop_ptr(&arg1);
    pop_ptr(&arg2);
    return map_fast(arg1, arg2);
  }
  else
  {
     // at least 1 arg must be present
     ref arg1;
     pop_ptr(&arg1);

     NEW_REF(pap_ref, void**, sizeof(void*) + sizeof(ref), pap_)

     struct info_table* pap_info = (struct info_table*)new(sizeof(struct info_table));
     pap_info->type = 4;
     pap_info->extra.pap_info = (struct pap){ .info_ptr = &map_info_table, .size = 1 };
     pap_[0] = pap_info;
     *(ref*)pap_ = arg1;
     return pap_ref;
  }
}

/* index_of :: Int# -> [a] -> a
   index_of x l = case x of
                    Cons
*/
ref index_of_fast(ref null)
{
    if(arg_satisfaction_check(sizeof(ref)*2))
}


ref head_case_cont(struct hash_map *bindings)
{
    GET_BINDING(l_ref, void**, l, 0, bindings)
    struct info_table *l_info = *(struct info_table **)l;

    if(l_info->type == 1)
    {
      assert(l_info->extra.constructor.con_num == 0);
      Cons *c = (Cons*)l;
      return c->value;
    }
    else return thunk_continuation(l_ref, head_case_cont, bindings, 0, l_ref);
}
/*
    head :: [a] -> a
    head l = case l of
                Cons x xs -> x
*/
ref head_fast(ref list)
{
    hash_map *bindings;
    init_bindings(&bindings);

    put_binding(bindings, 0, list);
    return head_case_cont(bindings);
}

ref head_slow(ref null)
{
    // no need to perform an argument satisfaction check because the only way this is called is when its applied to its argument
    ref arg;
    pop_ptr(&arg);
    return head_fast(arg);
}

ref tail_case_cont(hash_map* bindings)
{
   GET_BINDING(l_ref, void**, l, 0, bindings)
   struct info_table l_info = **(struct info_table **)l;

   if(l_info.type == 1)
   {
      assert(l_info.extra.constructor.con_num == 0);
      Cons *c = (Cons*)l;
      return c->next;
   }
   else return thunk_continuation(l_ref, tail_case_cont, bindings, 0, l_ref);
}

/*
  tail :: [a] -> [a]
  tail l = case l of
              Cons x xs -> xs
*/
ref tail_fast(ref list)
{
  hash_map *bindings;
  init_bindings(&bindings);

  put_binding(bindings, 0, list);

  return tail_case_cont(bindings);
}

ref tail_slow(ref list)
{
    ref arg_ref;
    pop_ptr(&arg_ref);
    return tail_fast(arg_ref);
}