#ifndef MMANAGER_H
#define MMANAGER_H

void* new(size_t);

void* new_calloc(int, size_t);

/* frees the memory allocated with the pointer and nulls out the pointer */
void delete(void **);

#endif
