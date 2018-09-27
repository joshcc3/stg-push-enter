#ifndef STACK_H
#define STACK_H

#include "static.h"


void* update_continuation(void *, void*);

void push_update_frame(void *);

void push_case_frame(void* (*)(struct hash_map*), int, struct hash_map *);

#endif
