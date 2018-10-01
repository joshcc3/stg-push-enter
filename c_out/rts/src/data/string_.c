#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "string_.h"
#include "containers/mmanager.h" 


err_t init_string_(const char * inp, int length, struct string_ ** result)
{
  assert(length >= 0);
  for(int i = 0; i < length - 1; i++) assert(inp[i]);

  char *new_char_array = (char*)new(sizeof(char) * length);
  memcpy(new_char_array, inp, length);
  struct string_ *tmp = (struct string_ *)new(sizeof(const struct string_));
  tmp->char_arr = new_char_array;
  tmp->length = length;

  *result = tmp;

  assert(string_length(*result) == length);

  return 0;
}


err_t deinit_string_(struct string_ ** result)
{
  assert(*result);

  void *tmp = (void*)((*result)->char_arr);
  delete(&tmp);
  delete((void **)result);

  assert(*result == NULL);

  return 0;
}


int string_length(const struct string_ * inp)
{
  assert(inp->length >= 0);
  return inp->length;
}

char charAt(const struct string_ * inp, int index)
{
  assert(inp->length >= 0 && inp->length > index && index >= 0);
  return inp->char_arr[index];
}

err_t substring_(const struct string_ * str, int start, int end, struct string_ ** output)
{
  assert(str && str->length >= 0 && start <= end && str->length > end && start >= 0 && output);

  init_string_(str->char_arr + start, end - start + 1, output);

  assert(string_length(*output) == end - start + 1);

  return 0;

}

bool equals(const struct string_ * str1, const struct string_ * str2)
{
  assert(str1 && str2);

  if(string_length(str1) != string_length(str2)) return false;

  for(int i = 0; i < string_length(str1); i++) 
  {
    if(charAt(str1, i) != charAt(str2, i)) return false;
  }

  return true;

}
