#ifndef MMANAGER_H
#define MMANAGER_H

void* new(int size);

/* frees the memory allocated with the pointer and nulls out the pointer */
void delete(void *ptr);

#endif