/* factor.c */
#include <math.h>
#include <stdio.h>
#include "Cbasic/factor.h"

/* factor */
void factor(int number, int n, int j) {
	int i;

	/* 1 is neither prime nor composite. */
	if (n == 1) {
		fprintf(stdout, "1 is a unit\n");
		return;
	}

	/* Determine the prime factors of n. */
	i = j;
	while (i <= (int)(sqrt((double)n))) {
		if (n % i == 0) {
			/* We have found a prime factor of n.
			 * Print it and factor n/i */
			fprintf(stdout, "%d\n", i);
			factor(number, (int)(n / i), i);
			return;
		} else {
			i++;
		}
	}

	/* If this point is reached, n is prime. */
	if (n == number) {
		fprintf(stdout, "%d is prime\n", number);
	} else {
		fprintf(stdout, "%d\n", n);
	}
}
