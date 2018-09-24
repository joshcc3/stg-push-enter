#include "stack.h"

void* update_continuation(struct update_frame *current_frame, void* value)
{
  current_frame->update_ref = value;
  su_register = current_frame->next_update_frame;
  stack_pointer += sizeof(struct update_frame);
  struct info_table *tbl = (struct info_table*)stack_pointer;

  void* (*continuation)(struct case_frame*, void*);
  if(tbl->type == 2)
  {
    continuation = tbl->extra.case_info.return_address;
  }
  else if(tbl->type == 3)
  {
    continuation = tbl->extra.update_info.return_address;
  }
  else assert(false);

  return continuation(stack_pointer, value);
}
