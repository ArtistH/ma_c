/* ex-2.c using DES in CBC mode */
#include <stdio.h>
#include <stdlib.h>
#include "cbc.h"

/* Define the size of text to encipher. */
#define TXTSIZ 1000

/* main */
int main() {
	unsigned char destmp[TXTSIZ];
	unsigned char desptx[TXTSIZ];
	unsigned char desctx[TXTSIZ];
	unsigned char deskey[8];
	int i = 0;

	/* Encipher some data using DES in CBC mode. */
	fprintf(stdout, "Enciphering with DES in CBC mode\n");
	deskey[0] = 0x01;
	deskey[1] = 0xf1;
	deskey[2] = 0x23;
	deskey[3] = 0x4a;
	deskey[4] = 0x1f;
	deskey[5] = 0x22;
	deskey[6] = 0x00;
	deskey[7] = 0xee;

	for (i = 0; i < TXTSIZ; i++) {
		destmp[i] = (i * 23) % 256;
	}

	cbc_encipher(destmp, desctx, deskey, TXTSIZ);
	cbc_decipher(desctx, desptx, deskey, TXTSIZ);

	for (i = 0; i < TXTSIZ; i++) {
		if (destmp[i] == desptx[i]) {
			fprintf(stdout, "destmp[%03d]=%02x, desctx[%03d]=%02x,"
					" desptx[%03d]=%02x (OK)\n",
					i, destmp[i], i, desctx[i], i, desptx[i]);
		} else {
			fprintf(stdout, "destmp[%03d]=%02x, desctx[%03d]=%02x,"
					" desptx[%03d]=%02x (ERROR)\n",
					i, destmp[i], i, desctx[i], i, desptx[i]);
		}
	}

	fprintf(stdout, "deskey: %02x %02x %02x %02x %02x %02x %02x %02x \n",
			deskey[0], deskey[1], deskey[2], deskey[3],
			deskey[4], deskey[5], deskey[6], deskey[7]);

	return 0;
}
