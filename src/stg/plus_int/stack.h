#ifndef STACK_H
#define STACK_H

#include "static.h"


struct ref update_continuation(struct ref);

struct ref case_continuation(struct ref);

void push_update_frame(struct ref);

void push_case_frame(struct ref (*)(struct hash_map*), int, struct hash_map *);

void push_ptr(struct ref);
void push_int(int);
void push_char(char);
void push_long(long);

void pop_ptr(struct ref*);

#endif
