#include "lib.h"

bool is_prime(uint64_t n)
{
	if (n <= 1)
		return false;
	else if (n <= 3)
		return true;
	else if ((n % 2) == 0 || (n % 3) == 0)
		return false;

	for (uint64_t i = 5; (i * i) <= n; i = (i + 6))
		if ((n % i) == 0 || (n % (i + 2)) == 0)
			return false;

	return true;
}

uint64_t next_prime (uint64_t n)
{
	if (n <= 1)
		return 2;

	while (!is_prime(++n));
	return n;
}
