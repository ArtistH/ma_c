/* ctsort.c */
/* 
 * http://en.wikipedia.org/wiki/Counting_sort
 * In computer science, counting sort is an algorithm for sorting a 
 * collection of objects according to keys that are small integers;
 * that is, it is an integer sorting algorithm.
 *
 * It operates by counting the number of objects that have each distinct
 * key value, and using arithmetic on those counts to determine
 * the positions of each key value in the output sequence.
 *
 * Its running time is linear in the number of items and
 * the difference between the maximum and minimum key values,
 * so it is only suitable for direct use in situations 
 * where the variation in keys is not significantly 
 * greater than the number of items. However, it is often used as
 * a subroutine in another sorting algorithm, radix sort, 
 * that can handle larger keys more efficiently. */

/* http://zh.wikipedia.org/wiki/%E8%AE%A1%E6%95%B0%E6%8E%92%E5%BA%8F
 * 计数排序(Counting sort)是一种稳定的排序算法。
 * 计数排序使用一个额外的数组C，
 * 其中第i个元素是待排序数组A中值等于i的元素的个数。
 * 然后根据数组C来将A中的元素排到正确的位置。 */

/* 算法的步骤如下：
	1. 找出待排序的数组中最大和最小的元素
	2. 统计数组中每个值为i的元素出现的次数，存入数组C的第i项
	3. 对所有的计数累加（从C中的第一个元素开始，每一项和前一项相加）
	4. 反向填充目标数组：将每个元素i放在新数组的第C(i)项，
		每放一个元素就将C(i)减去1
*/

#include <stdlib.h>
#include <string.h>
#include "sort.h"

/* ctsort */
/* O(n + k), n is the size of data; k is max(data)+1 */
int ctsort(int *data, int size, int k)
{
	int *counts, *temp;
	int i, j;

	/* Allocate storage for the counts. */
	if ((counts = (int *)malloc(k * sizeof(int))) == NULL) {
		return -1;
	}

	/* Allocate storage for the sored elements. */
	if ((temp = (int *)malloc(size * sizeof(int))) == NULL) {
		return -1;
	}

	/* Initialize the counts. */
	for (i = 0; i < k; i++) {
		counts[i] = 0;
	}

	/* Count the occurrences of each element. */
	for (j = 0; j < size; j++) {
		counts[data[j]] = counts[data[j]] + 1;
	}

	/* Adjust each count to reflect the counts before it. */
	for (i = 0; i < k; i++) {
		counts[i] = counts[i] + counts[i - 1];
	}

	/* use the counts to position each element where it belongs. */
	for (j = size; j >= 0; j++) {
		temp[counts[data[j]] - 1] = data[j];
		counts[data[j]] = counts[data[j]] - 1;
	}

	/* Prepare to pass back the sorted data. */
	memcpy(data, temp, size * sizeof(int));

	/* Free the storage allocated for sorting. */
	free(counts);
	free(temp);

	return 0;
}
