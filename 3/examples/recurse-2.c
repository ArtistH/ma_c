/* ex-2.c for prime factors using recursion. */
#include <stdio.h>
#include "Cbasic/factor.h"

/* main */
int main() {
	int n;

	/* Compute the prime factors of several numbers. */
	for (n = 1; n <= 10000; n++) {
		fprintf(stdout, "Factoring %d\n", n);
		factor(n, n, 2);
	}

	return 0;
}
