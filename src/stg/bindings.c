#include "bindings.h"
#include "containers/mmanager.h"
#include <assert.h>


struct arr_list *pointer_table;


void init_pointer_table(size_t size) { new_arrlist(16, &pointer_table); }

void* get_ref(struct ref ref) { return *(ref.ptr); }

void* upd_ref(struct ref ref, struct ref value) { *(ref.ptr) = get_ref(value); }

void new_ref(size_t size, struct ref* ref)
{
  void *value = new(size);
  int next_index = arrl_size(pointer_table);
  arrl_append(pointer_table, value);
  ref->ptr = pointer_table->wrapped->array + next_index;
}

void get_binding(struct hash_map *mp, int key, struct ref* value) 
{ 
  const void ***ref_ptr;
  hash_map_get(mp, (const void*)&key, (const void*)ref_ptr);
  value->ptr = ref_ptr;
}

void put_binding(struct hash_map *mp, int key, const struct ref value)
{
  int size = arrl_size(pointer_table);
  assert(*value.ptr >= pointer_table->wrapped->array && *value.ptr <= pointer_table->wrapped->array + size - 1 && (*(void***)value.ptr - (void**)pointer_table->wrapped->array)%sizeof(void *) == 0);
  int *key_ = (int*)new(sizeof(int));
  hash_map_put(&mp, (const void *)key_, (const void *)value.ptr);
}
