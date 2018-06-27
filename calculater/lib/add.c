#include <limits.h>
#include "../arithmetic.h"

int Add(int first_summand, int second_summand, int* result)
{
	if (first_summand > 0 && second_summand > 0 
		&& (INT_MAX - second_summand < first_summand) || first_summand < 0 &&
		second_summand < 0 && (INT_MIN - second_summand > first_summand)){
		return -1;
	}else{
		*result = first_summand + second_summand;
		return 0;
	}
}
