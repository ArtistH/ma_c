/* rxsort.c */
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"

/* rxsort */
int rxsort(int *data, int size, int p, int k) {
	int *counts;
	int *temp;
	int index;
	int pval;
	int i;
	int j;
	int n;

	/* Allocate storage for the counts. */
	if ((counts = (int *)malloc(k * sizeof(int))) == NULL) {
		return -1;
	}

	/* Allocate storage for the sorted elements. */
	if ((temp = (int *)malloc(size * sizeof(int))) == NULL) {
		return -1;
	}

	/* Sort from the least significant position to the most significant. */
	for (n = 0; n < p; n++) {
		/* Initialize the counts. */
		for (i = 0; i < k; i++) {
			counts[i] = 0;
		}

		/* Calculate the position value. */
		pval = (int)pow((double)k, (double)n);

		/* Count the occurrences of each digit value. */
		for (j = 0; j < size; j++) {
			index = (int)(data[j] / pval) % k;
			counts[index] = counts[index] + 1;
		}

		/* Adjust each count to reflect the counts before it. */
		for (i = 1; i < k; ++i) {
			counts[i] = counts[i] + counts[i - 1];
		}

		/* Use the counts to position each element where it belongs. */
		for (j = size - 1; j >= 0; j--) {
			index = (int)(data[j] / pval) % k;
			temp[counts[index] - 1] = data[j];
			counts[index] = counts[index] - 1;
		}

		/* Prepare to pass back the data as sorted thus far. */
		memcpy(data, temp, size * sizeof(int));
	}

	/* Free the storage allocated for sorting. */
	free(counts);
	free(temp);

	return 0;
}