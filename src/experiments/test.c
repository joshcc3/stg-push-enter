void f1(int a);
void f2(int a);


void f1(int a)
{
  f2(a - 1);
}

void f2(int b)
{
  f1(b + 1);
}

int main()
{
  return 0;
}
