/* bit.c */
#include <string.h>
#include "Cbasic/bit.h"

/* bit_get */
int bit_get(const unsigned char *bits, int pos) {
	unsigned char mask;
	int i;

	/* Set a mask for the bit to get. */
	mask = 0x80;
	for (i = 0; i < (pos % 8); i++) {
		mask = mask >> 1;
	}

	/* Get the bit. */
	return (((mask & bits[(int)(pos / 8)]) == mask) ? 1 : 0);
}

/* bit_set */
void bit_set(unsigned char *bits, int pos, int state) {
	unsigned char mask;
	int i;

	/* Set a mask for the bit to set. */
	mask = 0x80;
	for (i = 0; i < (pos % 8); i++) {
		mask = mask >> 1;
	}

	/* Set the bit. */
	if (state) {
		bits[pos / 8] = bits[pos / 8] | mask;
	} else {
		bits[pos / 8] = bits[pos / 8] & (~mask);
	}

	return;
}

/* bit_xor */
void bit_xor(const unsigned char *bits1, const unsigned char *bits2,
			 unsigned char *bitsx, int size) {
	int i;

	/* Compute the bitwise XOR (exclusive OR) of the two buffers. */
	for (i = 0; i < size; i++) {
		if (bit_get(bits1, i) != bit_get(bits2, i)) {
			bit_set(bitsx, i, 1);
		} else {
			bit_set(bitsx, i, 0);
		}
	}

	return;
}

/* bit_rot_left */
void bit_rot_left(unsigned char *bits, int size, int count) {
	int fbit = 0;
	int lbit;
	int i;
	int j;

	/* Rotate the buffer to the left the specified number of bits. */
	if (size > 0) {
		for (j = 0; j < count; j++) {
			for (i = 0; i <= ((size - 1) / 8); i++) {
				/* Get the bit about to be shifted off the current byte. */
				lbit = bit_get(&bits[i], 0);
				if (i == 0) {
					/* Save the bit shifted off the first byte for later. */
					fbit = lbit;
				} else {
					/* Set the rightmost bit of the previous byte to the
					 * leftmost bit about to be shifted off the current byte.*/
					bit_set(&bits[i - 1], 7, lbit);
				}
				/* Shift the current byte to the left. */
				bits[i] = bits[i] << 1;
			}
			/* Set the rightmost bit of the buffer to the bit shifted off
			 * the first byte. */
			bit_set(bits, size - 1, fbit);
		}
	}

	return;
}
