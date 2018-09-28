#include "stack.h"
#include <assert.h>
#include "containers/mmanager.h"
#include "containers/hash_map.h"
#include "stg/bindings.h"


struct ref update_continuation(struct ref value)
{
  struct update_frame* current_frame = (struct update_frame*)stack_pointer;
  upd_ref(current_frame->update_ref, value);
  su_register = (char*)current_frame->next_update_frame;
  stack_pointer += sizeof(struct update_frame);
  return value;
}

struct ref case_continuation(struct ref result)
{
  struct case_frame *frame = (struct case_frame *)stack_pointer;
  put_binding(&(frame->free_vars), frame->update_key, result);
  return frame->alternatives_evaluator(frame->free_vars);
}

void push_update_frame(struct ref update_ref) {
  stack_pointer -= sizeof(struct update_frame);
  struct update_frame *upd_frame = (struct update_frame*)(stack_pointer);
  upd_frame->update_ref = update_ref;
  upd_frame->next_update_frame = (struct update_frame*)su_register;
  su_register = stack_pointer;
  struct info_table *tbl =  (struct info_table*)new(sizeof(struct info_table));
  tbl->type = 3;
  tbl->extra.case_info.return_address = (void* (*)(void*))update_continuation;
  upd_frame->tbl = tbl;
}

struct ref case_cont(struct ref value)
{
  struct case_frame *frame = (struct case_frame *)stack_pointer;
  int k = frame->update_key;
  put_binding(&(frame->free_vars), k, value);
  // we dont need the case frame now..
  stack_pointer += sizeof(struct case_frame);

  return frame->alternatives_evaluator(frame->free_vars);

}

void push_case_frame(struct ref (*alternatives_evaluator)(struct hash_map*), int update_key, struct hash_map *bindings) {
      stack_pointer -= sizeof(struct case_frame);
      struct case_frame *case_frame = (struct case_frame*)(stack_pointer);
      case_frame->free_vars = bindings;
      case_frame->update_key = update_key;
      case_frame->alternatives_evaluator = alternatives_evaluator;
      struct info_table *case_info_ptr = (struct info_table*)new(sizeof(struct info_table));
      case_info_ptr->type = 2;
      case_info_ptr->extra.case_info.return_address = case_cont;
      case_frame->tbl = case_info_ptr;
}

void push_ptr(struct ref a)
{
  stack_pointer -= sizeof(struct ref);
  *(struct ref *)stack_pointer = a;
}


void push_int(int a)
{
  stack_pointer -= sizeof(int);
  *(int*)stack_pointer = a;
}


void pop_ptr(struct ref *res)
{
  *res = *(struct ref *)stack_pointer;
  stack_pointer += (sizeof(struct ref));
}
