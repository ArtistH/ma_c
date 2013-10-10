#include <stdio.h>
#include <string.h>
#include "Cbasic/encrypt.h"

static void print_des_8(unsigned char desstr[], char *des_name) {
	fprintf(stdout, "[%s]: %02x %02x %02x %02x %02x %02x %02x %02x\n",
			des_name, desstr[0], desstr[1], desstr[2], desstr[3],
			desstr[4], desstr[5], desstr[6], desstr[7]);
}

static void test_des() {
	unsigned char destmp[8];
	unsigned char desptx[8];
	unsigned char desctx[8];
	unsigned char deskey[8];

	fprintf(stdout, "Enciphering with DES\n");

	destmp[0] = 0xa9;
	destmp[1] = 0x10;
	destmp[2] = 0x11;
	destmp[3] = 0x38;
	destmp[4] = 0x93;
	destmp[5] = 0xca;
	destmp[6] = 0xb4;
	destmp[7] = 0xa1;

	deskey[0] = 0x01;
	deskey[1] = 0x1f;
	deskey[2] = 0x01;
	deskey[3] = 0x1f;
	deskey[4] = 0x01;
	deskey[5] = 0x0e;
	deskey[6] = 0x01;
	deskey[7] = 0x0e;

	fprintf(stdout, "Before enciphering\n");
	print_des_8(destmp, "destmp");
	print_des_8(deskey, "deskey");

	des_encipher(destmp, desctx, deskey);

	fprintf(stdout, "After enciphering\n");
	print_des_8(destmp, "destmp");
	print_des_8(desctx, "desctx");

	fprintf(stdout, "Before deciphering\n");
	print_des_8(desctx, "desctx");
	print_des_8(deskey, "deskey");

	des_decipher(desctx, desptx, deskey);

	fprintf(stdout, "After deciphering\n");
	print_des_8(desctx, "desctx");
	print_des_8(desptx, "desptx");

	fprintf(stdout, "\n");
	return;
}

static void test_rsa() {
	Huge rsatmp;
	Huge rsaptx;
	Huge rsactx;

	int i = 0;

	RsaPubKey rsapubkey;
	RsaPriKey rsaprikey;

	fprintf(stdout, "Enciphering with RSA\n");

	rsapubkey.e = 17;
	rsapubkey.n = 209;
	rsaprikey.d = 53;
	rsaprikey.n = 209;
	
	fprintf(stdout, "d=%lu, e=%lu, n=%lu\n",
			rsaprikey.d, rsapubkey.e, rsapubkey.n);

	for (i = 0; i < 128; i++) {
		rsatmp = i;
		rsa_encipher(rsatmp, &rsactx, rsapubkey);
		rsa_decipher(rsactx, &rsaptx, rsaprikey);

		if (rsatmp == rsaptx) {
			fprintf(stdout, "rsatmp=%5lu, rsactx=%5lu, rsaptx=%5lu (OK)\n",
					rsatmp, rsactx, rsaptx);
		} else {
			fprintf(stdout, "rsatmp=%5lu, rsactx=%5lu, rsaptx=%5lu (ERROR)\n",
					rsatmp, rsactx, rsaptx);
		}
	}

	return;
}

int main() {
	test_des();
	test_rsa();

	return 0;
}
