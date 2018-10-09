#define GET_BINDING(ref_name, val_type, val_name, binding_key, bindings) ref ref_name;\
      get_binding(bindings, binding_key, &ref_name); \
      val_type val_name = (val_type)get_ref(ref_name);

int main()
{
  int a = 1123;
  {
    int a= 10;
    printf("%d\n", a);    
  }
  printf("%d\n", a);
}

