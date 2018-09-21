#include "stack.h"
#include <assert.h>
#include "containers/mmanager.h"

void* update_continuation(void*current_frame_, void* value)
{
  struct update_frame* current_frame = (struct update_frame*)current_frame_;
  current_frame->update_ref = value;
  su_register = (char*)current_frame->next_update_frame;
  stack_pointer += sizeof(struct update_frame);
  struct info_table *tbl = (struct info_table*)(stack_pointer + sizeof(void*));

  if(tbl->type == 2)
  {
    struct case_frame *n_frame = (struct case_frame *)(stack_pointer + sizeof(struct case_frame));
    return (tbl->extra.case_info.return_address)(n_frame, value);
  }
  else if(tbl->type == 3)
  {
    struct update_frame *n_frame = (struct update_frame*)(stack_pointer + sizeof(struct update_frame));
    return (tbl->extra.update_info.return_address)(n_frame, value);
  }
  else assert(false);

  return NULL;

}

void push_update_frame(void *update_ref) {
      stack_pointer -= sizeof(struct update_frame);
      struct update_frame *upd_frame = (struct update_frame*)(stack_pointer);
      upd_frame->update_ref = update_ref;
      upd_frame->next_update_frame = (struct update_frame*)su_register;
      su_register = stack_pointer;
      struct info_table *tbl =  (struct info_table*)new(sizeof(struct info_table));
      tbl->type = 3;
      tbl->extra.case_info.return_address = (void* (*)(void*, void*))update_continuation;
      upd_frame->tbl = tbl;
}


void push_case_frame(void* (*continuation)(void*, void*), struct hash_map *bindings) {
      stack_pointer -= sizeof(struct case_frame);
      struct case_frame *case_frame = (struct case_frame*)(stack_pointer);
      case_frame->free_vars = bindings;
      struct info_table *case_info_ptr = (struct info_table*)new(sizeof(struct info_table));
      case_info_ptr->type = 2;
      case_info_ptr->extra.case_info.return_address = continuation;
      case_frame->tbl = case_info_ptr;
}
