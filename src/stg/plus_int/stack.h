#ifndef STACK_H
#define STACK_H

#include "static.h"


void* update_continuation(void *, void*);

void* case_continuation(void *);

void push_update_frame(void *);

void push_case_frame(void* (*)(struct hash_map*), int, struct hash_map *);

void push_ptr(void*);
void push_int(int);
void push_char(char);
void push_long(long);


#endif
