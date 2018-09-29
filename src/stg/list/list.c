#include <assert.h>

#include "list.h"
#include "containers/hash_map.h"
#include "stg/bindings.h"
#include "stg/plus_int/static.h"
#include "typeclasses.h"
#include "heap.h"

void init_list()
{
    cons_info_table = { .type = 1, .extra = { .constructor = { .arg_num = 2, .con_num = 0 } } };
    nil_info_table = { .type = 1, .extra = { .constructor = { .arg_num = 0, .con_num = 1 } } };

    arg_entry map_entries[2];
    map_entries[0] = { .pointer = true, .offset = 0 };
    map_entries[1] = { .pointer = true, .offset = sizeof(ref) };
    map_info_table = { .type = 0,
                       .extra.function = { .arity = 2, .slow_entry_point = map_slow },
                       .layout = { .num = 2, .entries = map_entries }
                      };

    nil_value = new_ref(sizeof(Nil), &nil_value);
    (Nil*)get_ref(nil_value)->info_ptr = &nil_info_table;
}

ref map_thunk1(ref bindings)
{
      ref f_ref;
      ref x_ref;
      get_binding(bindings, 1, &x_ref);

      get_binding(bindings, 0, &f_ref);

      void** f = (void**)get_ref(f_ref);
      struct info_table f_info = **(struct info_table**)f;
      if(f_info.type == 5)
      {
        push_update_frame(0);
        update_continuation(f_info.extra.thunk.return_address(f_ref));
        map_thunk1(bindings);
      }
      else if(f_info.type == 4)
      {
        push_ptr(x_ref);
        unroll_pap((void**)get_ref(f_ref));

        ref null;
        return f_info.function.slow_entry_point(null);
      }
      else if(f_info.type == 0)
      {
        push_ptr(x_ref);
        ref null;
        return f_info.function.slow_entry_point(null);
      }
      else assert(false);
}

ref map_thunk2(ref bindings) {
    ref a1;
    ref a2;
    get_binding(bindings, 0, &a1);
    get_binding(bindings, 0 ,&a2);
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
    ref list_ref;

    get_binding(bindings, 0, &func_ref);
    get_binding(bindings, 1, &list_ref);
    void **l_ = (void**)get_ref(list_ref);
    struct info_ptr *l_info = (struct info_ptr*)(*l_);
    if(l_info->type == 1)
    {
        if(l_info->extra.constructor.con_num == 0)
        {
            Cons *l = (Cons*)l;
            put_binding(bindings, 2, l->value);
            put_binding(bindings, 3, l->next);
            ref thunk1_ref = create_thunk(bindings, map_thunk1);
            put_binding(bindings, 4, thunk1_ref);

            ref thunk2_ref = create_thunk(bindings, map_thunk2);
            put_binding(bindings, 5, thunk2_ref);

            ref cons_ref;
            new_ref(sizeof(Cons), cons_ref);
            Cons *cons = (Cons *)get_ref(cons_ref);
            cons->info_ptr = cons_constructor_info_table;
            cons->value = thunk1_ref;
            cons->next = thunk2_ref;

            return cons;
         }
         else
         {
            assert(l_info->extra.constructor.con_num == 1);
            return nil_value;
         }
    }
    else return thunk_continuation(l_ref, map_case_cont, bindings, 1, 1);

    return cons;

}

// TODO: intialize the layout for every function info table, (plus_int, map)

ref map_fast(ref function, ref list)
{
    struct hash_map *bindings;
    init_hash_map(&bindings, 16, int_ptr_eq, int_ptr_obj);
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

     ref pap_ref;
     new_ref(&pap_ref);
     void **pap_ = (void**)get_ref(pap_ref);

     struct info_table* pap_info = (struct info_table*)new(sizeof(struct info_table));
     pap_info->type = 4;
     pap_info->extra.pap = { .info_ptr = &map_info_table, .size = 1 };
     pap_[0] = pap_info;
     *(ref*)pap_ = ref_arg1;
     return pap_ref;
  }
}

ref head_case_cont(struct hash_map *bindings)
{
    ref arg_ref;
    get_binding(bindings, 0, &arg_ref);
    void**l = (void**)get_ref(arg_ref);
    struct info_table *l_info = *(struct info_ptr **)l;
    if(l_info->type == 1)
    {
      assert(l_info->extra.constructor.con_num == 0);
      Cons *c = (Cons*)l;
      return c->value;
    }
    else return thunk_continuation(l_ref, head_case_cont, bindings, 0, 0);
}
/*
    head :: [a] -> a
    head l = case l of
                Cons x xs -> x
*/
ref head_fast(ref list)
{
    struct hash_map *bindings;
    init_hash_map(&bindings, 16, int_ptr_eq, int_ptr_obj);
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


ref tail_case_cont(ref bindings)
{
   ref l_ref;
   get_binding(bindings, 0, &l_ref);
   void** l = (void**)get_ref(l_ref);
   struct info_ptr l_info = **(struct info_ptr **)l;

   if(l_info.type == 1)
   {
      assert(l_info.extra.constructor.con_num == 1);
      Cons *c = (Cons*)l;
      return c->next;
   }
   else return thunk_continuation(l_ref, tail_case_cont, bindings, 0, 0);

}

/*
  tail :: [a] -> [a]
  tail l = case l of
              Cons x xs -> xs
*/
ref tail_fast(ref list)
{
  init_hash_map(&bindings, 16, &int_ptr_equals_typeclass, &int_ptr_obj_typeclass);
  put_binding(bindings, 0, list);

  tail_case_cont(bindings);
}

ref tail_slow(ref list)
{
    ref arg_ref;
    pop_ptr(&arg_ref);
    return tail_fast(arg_ref);
}