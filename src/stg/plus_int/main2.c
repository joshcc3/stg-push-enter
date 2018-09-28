#include "code.h"
#include "containers/mmanager.h"
#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "typeclasses.h"
#include "stack.h"
#include "static.h"
#include "stg/bindings.h"

void* c_cont(void *thunk_object)
{
  struct hash_map *bindings = *(struct hash_map**)(thunk_object + sizeof(void*));

  struct ref a;
  struct ref b;
  struct ref c;

  int a_key = 0;
  int b_key = 1;
  int c_key = 2;

  get_binding(bindings, a_key, (const struct ref*)&a);
  get_binding(bindings, b_key, (const struct ref*)&b);
  get_binding(bindings, c_key, (const struct ref*)&c);
  

  push_update_frame(c);

  push_ptr(b);
  push_ptr(a);
  
  return update_continuation(plus_int(NULL));

}

void* alternatives_evaluator(struct hash_map* bindings)
{

  int e_key = 3;
  struct ref e;
  get_binding(bindings, e_key, (const struct ref*)&e);
  
  struct info_table *e_info = *(struct info_table**)e;

  if(e_info->type == 1) {
    int d = *(int*)(e + sizeof(void*));
    printf("%d\n", d);
    return NULL;
  }
  else if(e_info->type == 5)
  {
    push_case_frame(alternatives_evaluator, 3, bindings);
    return case_continuation((e_info->extra.thunk_info.return_address)(e));
  }
  else { assert(false); }
}

void* main_function(void* _no_arg)
{
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

  struct i_hash* a = (struct i_hash*)new(sizeof(struct i_hash));
  a->info_ptr = &int_constructor_info_table;
  a->val = 1;

  struct i_hash* b = (struct i_hash*)new(sizeof(struct i_hash));
  b->info_ptr = &int_constructor_info_table;
  b->val = 2;

  struct info_table *info = (struct info_table *)new(sizeof(struct info_table));
  info->type = 5;
  info->extra.thunk_info.return_address = c_cont;
  
  void **c = new(sizeof(void*)*2);
  c[0] = (void*)info;
  c[1] = bindings;

  int a_key = 0;
  int b_key = 1;
  int c_key = 2;
  
  put_binding(bindings, a_key, (const void*)a);
  put_binding(bindings, b_key, (const void*)b);
  put_binding(bindings, c_key, c);

  struct ref tmp_;
  get_binding(bindings, a_key, (struct ref*)&tmp);
  void *tmp = *tmp_;
  assert(((struct i_hash*)tmp)->val == 1);

  push_case_frame(alternatives_evaluator, 3, bindings);

  push_ptr(c);
  push_ptr(c);
  return case_continuation(plus_int(NULL));

}
