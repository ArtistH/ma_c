/* hashpjw.c */
/* great for string hash */

//#include <hashpjw.h>
/* In practice, replace PRIME_TABLSIZ with the actual table size. */
#define PRIME_TABLSIZ 1000

unsigned int hashpjw(const void *key) {
	const char *ptr;
	unsigned int val;

	/* Hash the key by performing a number of bit operation on it. */
	val = 0;
	ptr = key;

	while (*ptr != '\0') {
		unsigned int tmp;
		val = (val << 4) + (*ptr);

		if (tmp = val & 0xf0000000) {
			val = val ^ (tmp >> 24);
			val = val ^ tmp;
		}

		ptr++;
	}

	return (val % PRIME_TABLSIZ);
}

