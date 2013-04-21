/* bit.h */

#ifndef BIT_H
#define BIT_H

/* Public Interface */

/* return 0,1
 * 获取缓冲区bits中处于位置pos的位的状态, 缓冲区最左边的位置为0. */
int bit_get(const unsigned char *bits, int pos);

/* 设置缓冲区bits中处于位置pos的位的状态,据state 0,1设置 */
void bit_set(unsigned char *bits, int pos, int state);

/* 计算异或值, 结果存放到bitsx, 调用者管理bitsx所需的空间
 * O(b), b为每个缓冲区中位的个数 */
void bit_xor(const unsigned char *bits1, const unsigned char *bits2, unsigned
	     char *bitsx, int size);

/* 轮转缓冲区bits(含size位), 将位值向左移count位
 * O(nb), b为每个缓冲区中位的个数, n为要轮转到左边的位数 */
void bit_rot_left(unsigned char *bits, int size, int count);

#endif
