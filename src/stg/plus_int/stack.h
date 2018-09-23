
struct update_frame {
  void (*return_address)(void*);
  void *heap_object;
  struct thunk *thunk;
  struct update_frame *next_update_frame;
};


void update_return_address(void *heap_object)
{
  perform the update; // what is the update?
  pop_frame_from_stack;
  return to the next frame..
}


struct arg {
  
};

struct case_frame {
  void (*return_address)(void*);
  struct free_var *saved;
};
