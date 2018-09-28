#include "bindings.h"
#include "containers/mmanager.h"


struct arr_list *pointer_table;


void init_pointer_table(sizt_t size) new_arrlist(16, &pointer_table);

void* get_ref(struct ref ref) { return *ref->ptr; }

void* upd_ref(struct ref ref, void *value) { *ref->ptr = value; }

void new_ref(size_t size, struct ref* ref)
{
  void *value = new(size);
  int next_index = arrl_size(pointer_table);
  arrl_append(pointer_table, value);
  ref->ptr = pointer_table->wrapped->array[next_index];
}

void get_binding_ref(struct hash_map *mp, int key, const struct ref* value) 
{ 
  const void **ref_ptr;
  hash_map_get(mp, (const void*)&key, (const void*)ref_ptr);
  value->ptr = ref_ptr;
}

void put_binding_ref(struct hash_map *mp, int key, const struct ref value)
{
  
  assert(value->ptr >= mp->wrapped->array && value->ptr <= mp->wrapped->array + size - 1 && (value->ptr - mp->wrapped->array)%sizeof(void *) == 0);
  int *key_ = (int*)new(sizeof(int));
  hash_map_put(&mp, (const void *)key_, (const void *)value->ptr);
}
