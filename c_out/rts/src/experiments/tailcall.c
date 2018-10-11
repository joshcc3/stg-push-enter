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

int jmp_func() __attribute__ ((noinline, noclone));

bool reg = true;
void* jmp_ptr;

int jmp_func()
{
  if(reg) 
    { 
      jmp_ptr = &&inner_label; 
      return 0;
    }

  printf("This should not be printed\n");
 inner_label:
  printf("Just after label2\n");
  return 100;

}



int goto_test()
{
  printf("Arrived at goto\n");
  goto *jmp_ptr;
  printf("Dont think this should get printed\n");
}



int main3()
{
  jmp_func();
  reg = false;

  int res = goto_test();
  printf("Value returned was %d, Just after goto test in main\n", res);

  return 0;
}


/*
 * Expected stack layout
 params in reverse order
 caller address
 base pointer
 local vars
 
 * So my strategy for implementing tail call optimization is:
 * get the return address location (rl) and the stack frame top (ft)
 
 * Then make the stack look like what the called function expects it to be:
  new params in reverse order
  caller address - preserved
  call into the new function


  Optimizations may play havoc with how arguments are passed
  Looking at gcc compiled code with and without optimization, rbp is preserved sometimes and not others.

 */

int main1()
{
  printf("%d\n", recursive_loop(20, 0));
  return 0;
}



// the case for thunks can be evaluated with relative ease by
// 

// How large is jmp state?
/*
 * you could probably implement tail recursion using long jmps.
 * you save the env when you first enter the function.
 * instead of calling the recursive function, you perform a long
 * jmp to the saved env
 */

/*
  Concerns - what about the contents of the stack?
  optimizing the C tail recursive functions for me is simple:

  It's optimizing other tail calls that are kinda difficult.
  Proposed plan: add the env to the continuation, do this by entering the function 
  
  
 */


jmp_buf env;
#define SPECIAL 123
int g()
{
  printf("Return address in g before calling setjmp: %p\n", __builtin_return_address(0));  
  int setJmpRes = !setjmp(env);
  printf("Return address in g after calling setjmp: %p\n", __builtin_return_address(0));
  if(!setJmpRes) { return 0; }


}

int f()
{
  return g();
}

int f_tailc()
{
  printf("Starting tailc\n");
  printf("Return address in f_tailc before calling g: %p\n", __builtin_return_address(0));
  printf("Calling g\n");
  g();
  printf("Return address in f_tailc after calling g: %p\n", __builtin_return_address(0));  
  printf("F is computing\n");

  sleep(1);
  longjmp(env, 1);

  printf("F after the longjmp");
  return 1;
}

int main_temp()
{
  printf("Calling f_tailc()\n");
  f_tailc();
  printf("Done computing tailc");
  printf("%d\n", f_tailc());
  return 1;
}


int main()
{
  printf("Calling main from main_temp\n");
  return main_temp();

}

