#include <stdio.h>



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

int main()
{
	printf("%d\n", recursive_loop(20, 0));
	return 0;
}