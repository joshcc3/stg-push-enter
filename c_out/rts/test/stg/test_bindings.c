#include "stg/plus_int/static.h"
#include "stg/bindings.h"
#include "containers/mmanager.h"
#include "stg/heap.h"
#include <stdio.h>


void test_unroll_pap()
{
    stack_pointer = (char*)new(128);
    su_register = stack_pointer;

    struct arg_entry entries[3];
    arg_entry[0] = (arg_entry) { .size = 4, .pointer = false, .offset = 0};
    arg_entry[1] = (arg_entry) { .size = 8, .pointer = true, .offset = 4};
    arg_entry[2] = (arg_entry) { .size = 4, .pointer = false, .offset = 12};

    struct info_table inf = { .type = 4, .extra.pap_info., .layout = { .num = 3, .entries = entries } };

    int heap_val = 30;
    void **pp = (void**)new(sizeof(void*)*2 + sizeof(int)*2);
    pp[0] = inf;
    pp++;
    *(int*)pp = 3;
    pp = (void*)((char*)pp + sizeof(int));
    *pp = &heap_val;
    pp++;
    *(int*)pp = 12;

    unroll_pap(pp);

    assert(stack_pointer);
}

int main()
{
    test_unroll_pap();
}