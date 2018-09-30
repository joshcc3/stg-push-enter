#include <assert.h>
#include "data/string_.h"
#include "static.h"
#include "typeclasses.h"
#include "stack.h"
#include "containers/mmanager.h"
#include "code.h"
#include "stg/bindings.h"


#define PTR_SIZE sizeof(void*)


void init_int()
{
  struct con con_info = { .arity = 1, .con_num = 0, .con_name = { .char_arr = "I#", .length = 2 } };
  struct arg_entry *con_entries = (struct arg_entry*)new(sizeof(struct arg_entry));
  struct arg_entry e1 = { .size = sizeof(int), .pointer = false, .offset = 0 };
  con_entries[0] = e1;

  struct layout con_layout = { .num = 1, .entries = con_entries };
  union info_table_u con_info_ = { .constructor = con_info };
  int_constructor_info_table.type = 1;
  int_constructor_info_table.extra = con_info_;
  int_constructor_info_table.layout = con_layout;

  struct arg_entry *plus_entries = (struct arg_entry*)new(sizeof(struct arg_entry)*2);
  struct arg_entry plus_entry1 = { .size = sizeof(ref), .pointer = true, .offset = 0 };
  struct arg_entry plus_entry2 = { .size = sizeof(ref), .pointer = true, .offset = sizeof(ref) };
  plus_entries[0] = plus_entry1;
  plus_entries[1] = plus_entry2;
  struct layout plus_layout = { .num = 2, .entries = plus_entries };

  struct fun plus_extra = {
    //.fast_entry_point = plus_int_fast,
    .slow_entry_point = plus_int_slow,
    .arity = 2

  };
  union info_table_u plus_extra_ = { .function = plus_extra };
  plus_info_table.type = 0;
  plus_info_table.extra = plus_extra_;
  plus_info_table.layout = plus_layout;
}

void c_int(int unboxed_value, ref *ref)
{
   new_ref(sizeof(void*) + sizeof(int), ref);
   void **ref_ = (void**)get_ref(*ref);
   ref_[0] = &int_constructor_info_table;
   *(int*)(ref_[1]) = unboxed_value;
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

struct ref alternatives_evaluator1(struct hash_map *bindings)
{

  struct ref b_;
  int b_key = 1;
  get_binding(bindings, b_key, &b_);

  void *b = get_ref(b_);
  struct info_table *b_info = *(struct info_table **)b;
  
  struct ref y_value_ref;
  new_ref(sizeof(int*), &y_value_ref);
  int* y_value = (int*)(get_ref(y_value_ref));

  /*
    I# x -> case b of
		           I# y -> let res = THUNK (x +# y) in 
			   let res2 = I# res in res2
  */
  if(b_info->type == 1)
  {
    int y_key = 3;

    *y_value = *(int*)(b + sizeof(void*));
    
    put_binding(bindings, y_key, y_value_ref);
    return continuation1(bindings);
  }
  else
  {
    assert(b_info->type == 5);
    
    // push the case continuation
    push_case_frame(continuation1, 3, bindings);
    
    // push the update continuation
    push_update_frame(b_);
    
    // set b to a blackhole in the heap
    b_info->type = 6;
    
    // enter the function for the thunk providing the necessary arguments - the address of the thunk
    struct ref b_computed = (b_info->extra.thunk_info.return_address)(b_);
    *y_value = *(int*)(get_ref(b_computed) + sizeof(void*));
    
    update_continuation(b_computed);
    return case_continuation(y_value_ref);
  }
}

struct ref plus_int_slow(ref null)
{
  assert (su_register - stack_pointer <= sizeof(void*)*2);
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

     ref pap_ref;
     new_ref(sizeof(int) + PTR_SIZE, &pap_ref);

     void **pap = (void**)get_ref(pap_ref);
     pap[0] = (void*)&int_constructor_info_table;
     *(int*)(pap + 1) = 1;
     ref* arg1_loc = (ref*)((char*)pap + PTR_SIZE + sizeof(int));
     *arg1_loc = arg1;

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

  void *a = get_ref(a_ref);

  int a_key = 0;
  int b_key = 1;
  put_binding(bindings, a_key, a_ref);
  put_binding(bindings, b_key, b_ref);


  struct info_table *a_info = *(struct info_table **)a;

  struct ref x_ref;
  new_ref(sizeof(void*), &x_ref);
  int *x = (int*)get_ref(x_ref);

  // We know that the arguments are of type Int which means they can only be the Constructor or Thunks that evaluate to the Constructor

  if(a_info->type == 1)
  {
    // The bindings dont escape this function (for unboxed values they will be copied so no need to worry about this stack getting cleaned up
    int x_key = 2;
    // The payload of a Constructor contains its arguments
    *x = *(int*)(a + sizeof(void*));

    put_binding(bindings, x_key, x_ref);
    return alternatives_evaluator1(bindings);
  }
  else
  {
    assert(a_info->type == 5);
    push_case_frame(alternatives_evaluator1, 2, bindings);
    push_update_frame(a_ref);
    a_info->type = 6;

    // update x
    struct ref a_computed = (a_info->extra.thunk_info.return_address(a_ref));
    *x = *(int*)(get_ref(a_computed) + sizeof(void*));

    update_continuation(a_computed);
    return case_continuation(x_ref);
  }

}
