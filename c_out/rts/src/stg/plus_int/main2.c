#include "code.h"
#include "containers/mmanager.h"
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "typeclasses.h"
#include "stack.h"
#include "static.h"
#include "stg/bindings.h"

struct ref c_cont(struct ref thunk_object_ref)
{
  void *thunk_object = get_ref(thunk_object_ref);
  struct hash_map *bindings = *(struct hash_map**)(thunk_object + sizeof(void*));

  struct ref a;
  struct ref b;
  struct ref c;

  int a_key = 0;
  int b_key = 1;
  int c_key = 2;

  get_binding(bindings, a_key, (struct ref*)&a);
  get_binding(bindings, b_key, (struct ref*)&b);
  get_binding(bindings, c_key, (struct ref*)&c);
  

  push_update_frame(c);

  return update_continuation(plus_int_fast(a, b));

}

struct ref alternatives_evaluator(struct hash_map* bindings)
{

  int e_key = 3;
  struct ref e_ref;
  get_binding(bindings, e_key, (struct ref*)&e_ref);
  void* e = get_ref(e_ref);
  
  struct info_table *e_info = *(struct info_table**)e;

  if(e_info->type == 1) {
    int d = *(int*)(e + sizeof(void*));
    printf("%d\n", d);
    return e_ref;
  }
  else if(e_info->type == 5)
  {
    push_case_frame(alternatives_evaluator, 3, bindings);
    return case_continuation((e_info->extra.thunk_info.return_address)(e_ref));
  }
  else { assert(false); }
}

struct ref main_function(struct ref _no_arg)
{
  init_int();

  // TODO Complete the implementation of the remaining stuff.
  // Build PAPs to see how they work
  // Next implement the prelude and try and create lists and sum lazy lists.
  /*
    -- Test to see that the thunk is only computed once
    let a = I# 1 in
    let b = I# 2 in
    let c = THUNK (plus_int a b) in
    case plus_int c c of
      I# d -> print d
    
   */
  
  struct hash_map *bindings;
  init_hash_map(&bindings, 16, &int_ptr_equals_typeclass, &int_ptr_obj_typeclass);

  struct ref a_ref;
  new_ref(sizeof(struct i_hash), &a_ref);
  struct i_hash *a = (struct i_hash*)get_ref(a_ref);
  a->info_ptr = &int_constructor_info_table;
  a->val = 1;

  struct ref b_ref;
  new_ref(sizeof(struct i_hash), &b_ref);
  struct i_hash* b = (struct i_hash*)get_ref(b_ref);
  b->info_ptr = &int_constructor_info_table;
  b->val = 2;

  struct info_table *info = (struct info_table *)new(sizeof(struct info_table));
  info->type = 5;
  info->extra.thunk_info.return_address = c_cont;
  
  struct ref c_ref;
  new_ref(sizeof(void*)*2, &c_ref);
  const void **c = (const void **)get_ref(c_ref);
  c[0] = (void*)info;
  c[1] = bindings;

  int a_key = 0;
  int b_key = 1;
  int c_key = 2;
  
  put_binding(bindings, a_key, a_ref);
  put_binding(bindings, b_key, b_ref);
  put_binding(bindings, c_key, c_ref);

  // TODO: Remove
  // Sanity check
  struct ref tmp_;
  get_binding(bindings, a_key, (struct ref*)&tmp_);
  void *tmp = get_ref(tmp_);
  assert(((struct i_hash*)tmp)->val == 1);

  push_case_frame(alternatives_evaluator, 3, bindings);

  return case_continuation(plus_int_fast(c_ref, c_ref));

}
