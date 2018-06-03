#include <stdlib.h>
#include <assert.h>

void* new_calloc(int num_items, size_t size)
{
  void *ptr = calloc(num_items, size);
  assert(ptr);
  return ptr;

}

void* new(size_t size)
{
    void *ptr = malloc(size);
    assert(ptr);
    return ptr;
}

void delete(void **ptr)
{
    assert(*ptr);
    free(*ptr);
    *ptr = NULL;
}
