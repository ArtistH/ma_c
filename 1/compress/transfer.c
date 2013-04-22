/* transfer.c */
#include <sys/types.h>
#include <sys/socket.h>

#include "compress.h"
#include "transfer.h"

/* send_comp O(n) */
/* s 是一个已经建立的套接字描述符
 * data 是要发送数据的缓冲区
 * size 是data的大小
 * flags 是要传入send的正常flags参数 */
int send_comp(int s, const unsigned char *data, int size, int flags)
{
	unsigned char *compressed;
	int size_comp;

	/* Compress the data. */
	if ((size_comp = Huffman_compress(data, &compressed, size)) < 0) {
		return -1;
	}

	/* Send the compressed data preceded by its size. */
	if (send(s, (char *)&size_comp, sizeof(int), flags) != sizeof(int)) {
		return -1;
	}

	if (send(s, (char *)compressed, size_comp, flags) != size_comp) {
		return -1;
	}

	/* Free thw buffer of compressed data. */
	free(compressed);

	return 0;
}

/* recv_temp  O(n) */
/* s 是一个已经建立的套接字描述符
 * data 是一个指向解压缩数据的指针
 * size 为data的大小(由Huffman_uncompress的返回值确定)
 * flags 是要传入recv的正常flags参数 */
int recv_comp(int s, unsigned char **data, int *size, int flags)
{
	unsigned char *compressed;
	int size_comp;
	
	/* Receive the compressed data preceded by its size. */
	if (recv(s, (char *)&size_comp, sizeof(int), flags) != sizeof(int)) {
		return -1;
	}

	if ((compressed = (unsigned char *)malloc(size_comp)) == NULL) {
		return -1;
	}

	if (recv(s, (char *)compressed, size_comp, flags) != size_comp) {
		free(compressed);
		return -1;
	}

	/* Uncompressed the data. */
	if ((*size = Huffman_uncompress(compressed, data)) < 0) {
		return -1;
	}

	/* Free the buffer of compressed data. */
	free(compressed);
	return 0;
}
