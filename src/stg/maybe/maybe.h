#ifndef MAYBE_H
#define MAYBE_H

#include "static.h"

struct nothing {
    struct info_table *;
};

struct just {
    struct info_table *;
    struct ref value;
};

struct ref maybe(struct ref (*)(struct ref), struct ref);

#endif