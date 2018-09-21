#include <stdio.h>
#include <string.h>

struct test {
  char *a;
  int x;
};

int main()
{
  struct test z = { .a = "The thing\0", .x = 13 };
  
  printf("%s.\n", z.a);

}

