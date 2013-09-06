/* hashpjw.c */
#include "hashpjw.h"

/* hashpjw */
int hashpjw(const void *key) {
	const char *ptr;
	int val = 0;

	/* Hash the key by performing a number of bit operations on it. */
	val = 0;
	ptr = key;
	int tmp;
	while (*ptr != '\0') {
		val = (val << 4) + (*ptr);
		tmp = (val & 0xf0000000);
		val = val ^ (tmp >> 24);
		val = val ^ tmp;
		ptr++;
	}

	/* In practice, replace PRIME_TBLSIZ with the actual table size. */
	return val & PRIME_TBLSIZ;
}
