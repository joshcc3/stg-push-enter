#include "bindings.h"
#include "containers/mmanager.h"

void get_binding(struct hash_map *mp, int key, const void**value)
{
  const void *tmp;
  hash_map_get(mp, (const void*)&key, &tmp);
  *value = *(const void**)tmp;
}

void put_binding(struct hash_map *mp, int key, const void* value)
{
  int *key_ = (int*)new(sizeof(int));
  *key_ = key;
  const void **ref = (const void**)new(sizeof(void**));
  *ref = value;
  hash_map_put(&mp, (const void*)key_, (const void*)ref);
}
