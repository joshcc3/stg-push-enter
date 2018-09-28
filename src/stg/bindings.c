#include "bindings.h"
#include "containers/mmanager.h"
#include <assert.h>


struct arr_list *pointer_table;


void init_pointer_table(size_t size) { new_arrlist(16, &pointer_table); }

void* get_ref(struct ref ref) { return *(ref.ptr); }

void upd_ref(struct ref ref, struct ref value) { *(ref.ptr) = get_ref(value); }

void new_ref(size_t size, struct ref* ref)
{
  void *value = new(size);
  int next_index = arrl_size(pointer_table);
  arrl_append(pointer_table, value);
  const void ** tmp = pointer_table->wrapped->array + next_index;
  ref->ptr = (void **)tmp;
}

void get_binding(struct hash_map *mp, int key, struct ref* value) 
{ 
  void **ref_ptr;
  hash_map_get(mp, (const void*)&key, (const void**)&ref_ptr);
  value->ptr = ref_ptr;
}

void put_binding(struct hash_map *mp, int key, const struct ref value)
{
  int size = arrl_size(pointer_table);
  void** v_ptr = value.ptr;
  void** array_start = (void**)pointer_table->wrapped->array;
  assert(v_ptr >= (void**)array_start && v_ptr <= array_start + size - 1 && ((char*)v_ptr - (char*)array_start)%sizeof(void **) == 0);
  int *key_ = (int*)new(sizeof(int));
  *key_ = key;
  hash_map_put(&mp, (const void *)key_, (const void *)value.ptr);
}
