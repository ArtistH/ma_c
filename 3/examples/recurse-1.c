/* ex-1.c for recursion and tail-recursion */
#include <stdio.h>
#include "Cbasic/fact.h"
#include "Cbasic/facttail.h"

/* main */
int main() {
	int n;

	/* Compute the factorials of several numbers. */
	for (n = 0; n < 14; ++n) {
		fprintf(stdout, "%2d! recursive: %-10d", n, fact(n));
		fprintf(stdout, "tail recursive: %-10d\n", facttail(n, 1));
	}

	return 0;
}
