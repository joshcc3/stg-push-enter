#include "heap.h"
#include "containers/mmanager.h"
#include <assert.h>

// bindings, thunk continuation
ref create_thunk(hash_map* bindings, ref (*return_address)(ref))
{
    struct info_table* thunk_info_ptr = (struct info_table*)new(sizeof(struct info_table));
    thunk_info_ptr->type = 5;
    thunk_info_ptr->extra.thunk_info.return_address = return_address;

    NEW_REF(thunk_ref, void**, sizeof(void*) * 2, thunk)

    thunk[0] = thunk_info_ptr;
    thunk[1] = (void*) bindings;

    return thunk_ref;
}

ref thunk_continuation(ref thunk_ref, ref (*case_cont)(struct hash_map*), struct hash_map* bindings, int case_key, ref update_ref)
{
    void** thunk = (void**)get_ref(thunk_ref);
    struct info_table *thunk_info = (struct info_table*)(*thunk);
    assert(thunk_info->type == 5);
    push_case_frame(case_cont, case_key, bindings);
    push_update_frame(update_ref);
    thunk_info->type = 6;
    ref result = update_continuation(thunk_info->extra.thunk_info.return_address(thunk_ref));

	__asm__ volatile (
	        "movq %0, %%rdi;\n\t"
	        "movq %%rbp, %%rsp;\n\t"
	        "popq %%rbp;\n\t"
	        :
	        : "r"(res)
	        : "rdi"
	);
	goto *(void*)case_continuation;

}


void unroll_pap(void **pap)
{
  // We unroll the pap from back to front (because args must be pushed in that order).

  struct pap p_info = ((struct info_table*)pap[0])->extra.pap_info;
  struct info_table *f_info = (struct info_table *)(p_info.info_ptr);
  arg_entry *initial_arg_entry = f_info->layout.entries;
  arg_entry *arg_entry = initial_arg_entry + f_info->extra.function.arity - 1;
  int max_offset = arg_entry->offset;

  const char *pap_arg_start = (const char*)(pap + 1);
  const char *pap_entry = pap_arg_start;
  // get the nth byte from the start of the second word of the pap (which is where the args start).
  for(int i = 0; i < p_info.size; ++i)
  {
    assert(arg_entry->offset == max_offset - (pap_entry - pap_arg_start));
    if(!arg_entry->pointer)
    {
      push_int(*(int*)pap_entry);
      pap_entry += sizeof(int);
    }
    else
    {
        push_ptr(*(ref*)pap_entry);
        pap_entry += sizeof(ref);
    }
    arg_entry--;
  }

}
