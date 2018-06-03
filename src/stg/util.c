#include <assert.h>

int zero_based_index(int ix, int len)
{
    assert(len >= 0);

    if(len == 0) return ix;

    return (ix + len) % len;

}
