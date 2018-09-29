#include "heap.h"

// bindings, thunk continuation
ref create_thunk(struct hash_map* bindings, ref (*return_address)(ref))
{
    struct info_table* thunk_info_ptr = (struct info_table*)new(sizeof(struct info_table));
    thunk_info_ptr->type = 5;
    thunk_info_ptr->extra.thunk.return_address = return_address;

    ref thunk_ref;
    new_ref(&new_ref);
    void** thunk = (void**)get_ref;
    thunk[0] = thunk_info_ptr;
    thunk[1] = bindings;

    return thunk_ref;
}

ref thunk_continuation(ref thunk_ref, ref (*case_cont)(struct hash_map*), struct hash_map* bindings, int case_key, int update_key)
{
    void** thunk = (void**)get_ref(thunk_ref);
    struct info_table thunk_info = *(struct info_table*)(*thunk);
    assert(thunk_info.type == 5);
    push_case_frame(case_cont, case_key, bindings);
    push_update_frame(update_key);
    return case_continuation(update_continuation(l_info->extra.thunk.return_address(thunk_ref)));
}


void unroll_pap(void **pap)
{
  // We unroll the pap from back to front (because args must be pushed in that order).
  struct info_table *pap_info = (struct info_table *)pap[0];
  struct info_table *f_info = (struct info_table *)(pap_info->extra.pap.info_ptr);
  arg_entry *arg_entry = f_info->layout.entries[pap_info->size - 1];

  // get the nth byte from the start of the second word of the pap (which is where the args start).
  const char *pap_entry = (char*)(pap + 1)[arg_entry->offset];
  for(int i = 0; i < pap_info->size; ++i)
  {
    if(!arg_entry->pointer)
    {
      push_int(*(int*)pap_entry);
      pap_entry -= sizeof(int);
    }
    else
    {
        push_ptr(*(ref*)pap_entry);
        pap_entry -= sizeof(ref);
    }
    else assert(false);
    arg_entry--;
  }
}
