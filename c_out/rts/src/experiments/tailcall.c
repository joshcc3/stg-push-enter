#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>


/*
 * 
 */
int recursive_loop(int n, int sum)
{
	if(n <= 0) return sum;
	else
	{
		int new_sum = sum + n;
		int new_n = n - 1;
		return recursive_loop(new_n, new_sum);
	}
}

// 261945 - and then it segfaults (4MB of stack space)
int f(int x, int y, int z, int a, int b)
{
  if(x % 100000 == 0) printf("%d\n", x);  
  return f(++x, y, z, a, b);
}
// use the sysv_abi to ensure compatibility with windows, amd
int f_opt(int x) __attribute__ ((noinline, noclone, sysv_abi));
int f_opt(int x)
{
  x++;
  if(x % 100000 == 0) printf("%d\n", x);
  __asm__ volatile (
		    "movq %%rbp, %%rsp;\n\t"
		    "popq %%rbp;\n\t"
		    "movl %0, %%edi;\n\t"
		    :
		    :"r"(x)
		    );
  goto *(void*)f_opt;

/*  __asm__ volatile (
		     "movq %%rbp, %%rsp;\n\t"
		     "popq %%rbp;\n\t"
		     "movl %0, %%edi;\n\t"
		     :
		     :"r" (x));

  goto *(void*)f_opt;
*/
}
int main()
{

  printf("The result is %d\n", f_opt(0));
}


