#include "stg/static.h"

typedef struct Cons {
    struct info_table *ptr;
    void* value;
    struct list *next;
} Cons;

typedef struct Nil {
    struct info_table *ptr;
} Nil;

ref head_slow(ref);
ref head_fast(ref);

ref tail_slow(ref);
ref tail_fast(ref);

ref isNull_slow(ref);
ref isNull_fast(ref);

ref map_slow(ref);
ref map_fast(ref, ref);

ref foldl_slow(ref);
ref foldl_fast(ref, ref, ref);
