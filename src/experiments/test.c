#include <stdio.h>
#include <string.h>

struct test {
  char *a;
  int x;
};

typedef struct ref {
    void **ptr;
} ref;

int main()
{
    struct ref a;
    ref b;
}

