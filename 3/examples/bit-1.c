/* ex-1.c for using bit-operations */
#include <stdio.h>
#include <string.h>
#include "Cbasic/bit.h"

/* print_bits only used for this time. */
static void print_bits(const unsigned char bits[], const char* bits_name) {
	fprintf(stdout, "[%s]: %02x %02x %02x %02x   %02x %02x %02x %02x \n",
			bits_name, bits[0], bits[1], bits[2], bits[3], bits[4],
			bits[5], bits[6], bits[7]);
	return;
}

/* main */
int main() {
	unsigned char bits1[8];
	unsigned char bits2[8];
	unsigned char bits3[8];
	int i;

	/* Perform some bit operations using 64-bit buffers. */
	for (i = 0; i < 8; ++i) {
		bits1[i] = 0x00;
		bits2[i] = 0x00;
		bits3[i] = 0x00;
	}

	fprintf(stdout, "Initially\n");
	print_bits(bits1, "bits1");
	print_bits(bits2, "bits2");

	bit_set(bits1, 15, 1);
	bit_set(bits1, 16, 1);
	bit_set(bits1, 32, 1);
	bit_set(bits1, 63, 1);
	bit_set(bits2,  0, 1);
	bit_set(bits2, 15, 1);

	fprintf(stdout, "After setting bits 15, 16, 32, 63 of bits1 and "
			" 00, 15 of bits2\n");
	print_bits(bits1, "bits1");
	print_bits(bits2, "bits2");

	fprintf(stdout, "Bit 63 of bits is %d\n", bit_get(bits1, 63));
	fprintf(stdout, "Bit 62 of bits is %d\n", bit_get(bits1, 62));
	fprintf(stdout, "Bit 00 of bits is %d\n", bit_get(bits1,  0));
	fprintf(stdout, "Bit 01 of bits is %d\n", bit_get(bits1,  1));

	bit_xor(bits1, bits2, bits3, 32);
	fprintf(stdout, "bits3 is bits1 XOR bits2 (32 bits)\n");
	print_bits(bits3, "bits3");

	bit_xor(bits1, bits2, bits3, 64);
	fprintf(stdout, "bits3 is bits1 XOR bits2 (64 bits)\n");
	print_bits(bits3, "bits3");

	bit_rot_left(bits1, 64, 1);
	fprintf(stdout, "After bits1 left x 1 (64 bits)\n");
	print_bits(bits1, "bits1");

	bit_rot_left(bits2, 64, 1);
	fprintf(stdout, "After bits2 left x 1 (64 bits)\n");
	print_bits(bits2, "bits1");

	bit_rot_left(bits2, 16, 7);
	fprintf(stdout, "After bits2 left x 7 (16 bits)\n");
	print_bits(bits2, "bits2");

	bit_rot_left(bits2, 8, 2);
	fprintf(stdout, "After bits2 left x 2 (8 bits)\n");
	print_bits(bits2, "bits2");

	for (i = 0; i < 8; i++) {
		bits2[i] = 0x00;
	}
	bit_set(bits2,  0, 1);
	bit_set(bits2,  3, 1);
	bit_set(bits2,  8, 1);
	bit_set(bits2, 27, 1);
	fprintf(stdout, "After clearing and setting bits 0, 3, 8, 27 of bits2\n");
	print_bits(bits2, "bits2");

	bit_rot_left(bits2, 11, 6);
	fprintf(stdout, "After rotating bits2 left x 6 (11 bits)\n");
	print_bits(bits2, "bits2");

	for (i = 0; i < 8; ++i) {
		bits2[i] = 0x00;
	}
	bit_set(bits2,  0, 1);
	bit_set(bits2,  3, 1);
	bit_set(bits2,  8, 1);
	bit_set(bits2, 27, 1);
	fprintf(stdout, "After clearing and setting bits 0, 3, 8, 27 of bits2\n");
	print_bits(bits2, "bits2");

	bit_rot_left(bits2, 28, 4);
	fprintf(stdout, "After rotating bits2 left x 4 (28 bits)\n");
	print_bits(bits2, "bits2");

	return 0;
}
