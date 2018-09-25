#include "stack.h"

void* update_continuation(struct update_frame *current_frame, void* value)
{
  current_frame->update_ref = value;
  su_register = current_frame->next_update_frame;
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
