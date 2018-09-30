#include <assert.h>
#include "data/string_.h"
#include "static.h"
#include "typeclasses.h"
#include "stack.h"
#include "containers/mmanager.h"
#include "code.h"
#include "stg/bindings.h"
#include "stg/heap.h"


#define PTR_SIZE sizeof(void*)


void init_int()
{
  struct arg_entry *con_entries = (struct arg_entry*)new(sizeof(struct arg_entry));
  con_entries[0] = (arg_entry) { .size = sizeof(int), .pointer = false, .offset = 0 };

  int_constructor_info_table.type = 1;
  int_constructor_info_table.extra.constructor = (struct con) {
    .arity = 1,
    .con_num = 0,
    .con_name = { .char_arr = "I#", .length = 2 }
  };
  int_constructor_info_table.layout = (struct layout) { .num = 1, .entries = con_entries };

  struct arg_entry *plus_entries = (struct arg_entry*)new(sizeof(struct arg_entry)*2);
  plus_entries[0] = (arg_entry) { .size = sizeof(ref), .pointer = true, .offset = 0 };
  plus_entries[1] = (arg_entry) { .size = sizeof(ref), .pointer = true, .offset = sizeof(ref) };

  plus_info_table.type = 0;
  plus_info_table.extra.function = (struct fun) { .slow_entry_point = plus_int_slow, .arity = 2 };
  plus_info_table.layout = (struct layout) { .num = 2, .entries = plus_entries };
}

void c_int(int unboxed_value, ref *ref)
{
   new_ref(sizeof(void*) + sizeof(int), ref);
   void **ref_ = (void**)get_ref(*ref);
   ref_[0] = &int_constructor_info_table;
   *(int*)(ref_ + 1) = unboxed_value;
}

// TODO: I dont think you can free stuff that is on the stack so the hash map which is full of references to the stack will need
// to be taken care of

// TODO: Need to perform stack overflow and underflow checks whenever you push/pop from the stack


// TODO: There's no need to use a hashmap to save bindings, you add them to the payload of the case continuation and free the block when the case continuation is popped

/*
  
  data Int = I# Int#
 
  print_int :: Int# -> IO ()



  plus :: Int -> Int -> Int
  plus a b = case a of
                I# x -> case b of
		           I# y -> let res2 = x +# y 
			     in I# res2

  main = let z = I# 1 in
         let y = I# 2 in
	 case (plus z y) of
	    I# x -> case x of
	              v -> print_int v

 */

struct ref thunk1_cont(struct ref thunk_object_ref) 
{
  void *thunk_object = get_ref(thunk_object_ref);
  struct hash_map *bindings = (struct hash_map *)(((const void **)thunk_object)[1]);
  struct ref vx;
  struct ref vy;
  get_binding(bindings, 2, (struct ref*)&vx);
  get_binding(bindings, 3, (struct ref*)&vy);
  struct ref result_ref;
  new_ref(sizeof(int), &result_ref);
  int *result = (int*)get_ref(result_ref);
  *result = *(int*)get_ref(vx) + *(int*)get_ref(vy);
  return result_ref;
}


struct ref continuation1(struct hash_map *bindings) {
  
  // let res = THUNK (x +# y) in 
  // let res2 = I# res in res2
  struct info_table *thunk1_info_table = (struct info_table *)new(sizeof(void *));
  thunk1_info_table->type = 5;
  struct layout layout_tmp = { .num = 1 };
  thunk1_info_table->layout = layout_tmp; // TODO: Need to intialize the layout
  thunk1_info_table->extra.thunk_info.return_address = thunk1_cont;

  struct ref thunk1_ref;
  new_ref(sizeof(void**)*2, &thunk1_ref);
  void**thunk1 = (void**)get_ref(thunk1_ref);
  thunk1[0] = (void*)thunk1_info_table;
  thunk1[1] = (void*)bindings;	
  
  int res_key = 4;

  put_binding(bindings, res_key, thunk1_ref);

  struct ref constructor1_ref;
  new_ref(sizeof(struct i_hash), &constructor1_ref);
  struct i_hash *constructor1 = (struct i_hash *)get_ref(constructor1_ref);

  constructor1->info_ptr = &int_constructor_info_table;
  struct ref tmp;
  get_binding(bindings, res_key, &tmp);
  struct info_table *tbl = *(struct info_table **)get_ref(tmp);
  struct ref thunk_result = (tbl->extra.thunk_info.return_address)(tmp);
  constructor1->val = *(int*)(get_ref(thunk_result));
  
  int res2_key = 5;
  put_binding(bindings, res2_key, constructor1_ref);
  // Is it safe to deinit 'bindings' here?

  return constructor1_ref;

}

ref alternatives_evaluator2(struct hash_map *bindings)
{

  GET_BINDING(b_ref, void*, b, 1, bindings)
  struct info_table *b_info = *(struct info_table **)b;

  /*
    I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in
			   let res2 = I# res in res2
  */
  if(b_info->type == 1)
  {
    NEW_REF(y_ref, int*, sizeof(int*), y)
    *y = *(int*)(b + sizeof(void*));

    put_binding(bindings, 3, y_ref);
    return continuation1(bindings);
  }
  // thunk object, case continuation, current bindings, update key for the case, update key for the update cont
  else return thunk_continuation(b_ref, alternatives_evaluator2, bindings, 1, b_ref);

}


ref alternatives_evaluator1(struct hash_map *bindings)
{
  GET_BINDING(a_ref, void*, a, 0, bindings)
  struct info_table *a_info = *(struct info_table **)a;


  // We know that the arguments are of type Int which means they can only be the Constructor or Thunks that evaluate to the Constructor

  if(a_info->type == 1)
  {
    NEW_REF(x_ref, int*, sizeof(int*), x)
    // The bindings dont escape this function (for unboxed values they will be copied so no need to worry about this stack getting cleaned up
    // The payload of a Constructor contains its arguments
    *x = *(int*)(a + sizeof(void*));

    put_binding(bindings, 2, x_ref);
    return alternatives_evaluator2(bindings);
  }

  else return thunk_continuation(a_ref, alternatives_evaluator1, bindings, 0, a_ref);
}


struct ref plus_int_slow(ref null)
{
  arg_satisfaction_check(sizeof(void*)*2);
  if(su_register - stack_pointer == sizeof(void*)*2)
  {
      ref a_ref;
      pop_ptr(&a_ref);

      ref b_ref;
      pop_ptr(&b_ref);

      return plus_int_fast(a_ref, b_ref);
  }
  else
  {
     // at least 1 arg must be present

     ref arg1;
     pop_ptr(&arg1);

     NEW_REF(pap_ref, void**, sizeof(void*)*2, pap)
     NEW(struct info_table, pap_info);


     pap_info->type = 4;
     pap_info->extra = (union info_table_u){ .pap_info = { .info_ptr = &plus_info_table, .size = 1 } };
     // pap_info->layout = {};
     pap[0] = (void*)pap_info;
     *(ref*)(pap + 1) = arg1;

     return pap_ref;
  }
}


// we expect all of our arguments to be passed on the stack
struct ref plus_int_fast(ref a_ref, ref b_ref)
{
  // the fast entry point (no argument satisfaction check)
  /*

plus_int a b = case a of
    I# x -> case b of
             I# y -> let res = THUNK (x +# y) in 
	             let res2 = I# res in res2
    
case (plus_unboxed 1 2) of
  case I# z -> case z of 
                   a -> print_int a

  */  
  // fast entry point for known calls

  struct hash_map *bindings = NULL;
  init_hash_map(&bindings, 16, &int_ptr_equals_typeclass, &int_ptr_obj_typeclass);
  // the garbage collector will need to refcount this bindings map and free it when it is done - not sure how this would be implemented.

  int a_key = 0;
  int b_key = 1;
  put_binding(bindings, a_key, a_ref);
  put_binding(bindings, b_key, b_ref);

  return alternatives_evaluator1(bindings);

}
