#include "stg/static.h"

typedef struct Cons {
    struct info_table *info_ptr;
    ref value;
    ref next;
} Cons;


typedef struct Nil {
    struct info_table *info_ptr;
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

struct info_table cons_info_table;
struct info_table nil_info_table;
struct info_table map_info_table;
struct info_table head_info_table;
struct info_table tail_info_table;
ref nil_value;