#include "stack.h"
#include <stdlib.h>
#include <assert.h>
#include "containers/mmanager.h"
#include "containers/hash_map.h"
#include "stg/bindings.h"
#include <stdio.h>

#define STACK_OVERFLOW_CHECK if(stack_pointer < stack_bottom) \
    {\
      if(CUR_STACK_SIZE >= INITIAL_STACK_SIZE * 1024L * 5L) \
      {								\
    char message[128];						\
    sprintf(message, "Stack Overflow Exception: Stack pointer - %p, Stack_bottom - %p", stack_pointer, stack_bottom); \
    perror(message);						\
    assert(false);							\
      }									\
      else								\
    {								\
      perror("Haven't added stack growth yet because we need to learn to walk first"); \
      assert(false);						\
    }								\
    }									\
      // Growing the stack is a bit troublesome because you need to walk all update continuations and update the saved value of su.
      /*int old_stack_size = CUR_STACK_SIZE;			\
      CUR_STACK_SIZE *= 2;						\
      char *new_stack_bottom = allocate_stack(CUR_STACK_SIZE);	\
      memcpy(CUR_STACK_SIZE >> 1 + new_stack_bottom, stack_bottom, CUR_STACK_SIZE >> 1);\
      stack_pointer
      stack_bottom = new_stack_bottom;
          */


char* allocate_stack(int stack_size) { return (char*)new(stack_size); }

char* stack_bottom;
char* stack_top;
long CUR_STACK_SIZE = INITIAL_STACK_SIZE;

void init_stack()
{
  stack_bottom = allocate_stack(INITIAL_STACK_SIZE);
  stack_pointer = stack_bottom + INITIAL_STACK_SIZE;
  stack_top = stack_bottom + CUR_STACK_SIZE;

  su_register = stack_pointer;
}

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
  void *free_vars = (void*)frame->free_vars;
  put_binding(free_vars, frame->update_key, result);
  stack_pointer += sizeof(struct case_frame);
  void *jmpaddr = (void*)frame->alternatives_evaluator;
  __asm__ volatile (
		    "movq %0, %%rdi;\n\t"
		    "movq %1, %%rsi;\n\t"
		    "movq %%rbp, %%rsp;\n\t"
		    "popq %%rbp;\n\t"
		    "jmp *%1;\n\t"
		    :
		    : "r"(free_vars),  "r"(jmpaddr)
		    : "rdi",  "rsi"
		    );
}

void push_update_frame(struct ref update_ref) {
  stack_pointer -= sizeof(struct update_frame);
  STACK_OVERFLOW_CHECK  
  struct update_frame *upd_frame = (struct update_frame*)(stack_pointer);
  upd_frame->update_ref = update_ref;
  upd_frame->next_update_frame = (struct update_frame*)su_register;
  su_register = stack_pointer;
  struct info_table *tbl =  (struct info_table*)new(sizeof(struct info_table));
  tbl->type = 3;
  tbl->extra.case_info.return_address = (struct ref (*)(struct ref))update_continuation;
  upd_frame->tbl = tbl;
}

static struct ref case_cont(struct ref value)
{
  struct case_frame *frame = (struct case_frame *)stack_pointer;
  int k = frame->update_key;
  put_binding(frame->free_vars, k, value);
  // we dont need the case frame now..
  stack_pointer += sizeof(struct case_frame);

  return frame->alternatives_evaluator(frame->free_vars);

}

void push_case_frame(struct ref (*alternatives_evaluator)(struct hash_map*), int update_key, struct hash_map *bindings) {
      stack_pointer -= sizeof(struct case_frame);
      STACK_OVERFLOW_CHECK
      
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
  STACK_OVERFLOW_CHECK
  *(struct ref *)stack_pointer = a;
}


void push_int(int a)
{
  stack_pointer -= sizeof(int);
  STACK_OVERFLOW_CHECK  
  *(int*)stack_pointer = a;
}


void pop_ptr(struct ref *res)
{
  *res = *(struct ref *)stack_pointer;
  stack_pointer += (sizeof(struct ref));
}

void pop_int(int *res)
{
    *res = *(int*)stack_pointer;
    stack_pointer += (sizeof(int));
}
