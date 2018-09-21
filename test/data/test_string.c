#include "data/string_.h"
#include <assert.h>
#include <stdio.h>

void test_simple_string_()
{

  char *str = "0123456789\0";
  struct string_ *result;
  init_string_(str, 10, &result);
  
  assert(string_length(result) == 10);
  for(int i = 0; i < 10; i++) assert(charAt(result, i) == '0' + i);

  assert(result->char_arr != str);

  deinit_string_(&result);

}

void test_substring_and_equals()
{
  char *str = "0123456789\0";
  struct string_ *result;

  init_string_(str, 10, &result);
  
  struct string_* result2;
  substring_(result, 3, 6, &result2);

  char *str2 = "3456\0";
  struct string_ *check_string_;
  init_string_(str2, 4, &check_string_);
  
  assert(equals(result2, check_string_));

  struct string_ *result3;
  substring_(result, 3, 6, &result3);

  assert(equals(result2, result3));

  assert(equals(result3, check_string_));

  deinit_string_(&result);
  
}


int main()
{
  test_simple_string_();
  printf("Tested a simple string\n");
  test_substring_and_equals();
  printf("Tested substring and equals\n");
}
