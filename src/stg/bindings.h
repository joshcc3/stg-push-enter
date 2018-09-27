#ifndef BINDINGS_H
#define BINDINGS_H

#include "containers/hash_map.h"


void get_binding(struct hash_map *mp, int key, const void**value);
void put_binding(struct hash_map *mp, int key, const void*value);

#endif
