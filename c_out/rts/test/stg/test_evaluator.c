

int test_variable()
{

  
  obj = primop("print_int", lit(1));
  entry = { .name = "main", .object = obj };

  program = struct program();
  program.add_entry(entry);

  eval(program);

}
