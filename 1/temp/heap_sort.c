/**
 * usage: heap_sort n
 * n is the number of the array that will be sorted, 10 by default
 * svn checkout http://exercise.googlecode.com/svn/trunk/ exercise-read-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_arr(int *arr, int n);
void heap_sort(int *arr, int n);

int main(int argc, char **argv)
{
	int n;
	srand(time(0));
	if (argc == 1) {
		n = 10;
	} else {
		n = atoi(argv[1]);
	}
	int *arr = (int *)malloc(sizeof(int) * n);
	int i;
	for (i = 0; i < n; i++) {
		arr[i] = (int)(rand() % 100);
	}
	printf("ini_array: ");
	print_arr(arr, n);
	heap_sort(arr, n);
	printf("sorted_arr: ");
	print_arr(arr, n);
	free(arr);
}

void print_arr(int *arr, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (!i) {
			printf("%d", arr[i]);
		} else {
			printf(" %d", arr[i]);
		}
	}
	printf("\n");
}

void heap_sort(int *arr, int n)
{
	int last_parent = n / 2 - 1;
	int i;
	for (i = last_parent; i >= 0; i--) {
		int parent = i;
		while (1) {
			int max, max_pos;
			if (2 * (parent + 1) < n) {
				if (arr[2 * parent + 1] > arr[2 * (parent + 1)]) {
					max_pos = 2 * parent + 1;
					max = arr[2 * parent + 1];
				} else {
					max_pos = 2 * (parent + 1);
					max = arr[2 * (parent + 1)];
				}
			} else if (2 * parent + 1 < n) {
				max_pos = 2 * parent + 1;
				max = arr[2 * parent + 1];
			} else {
				break;
			}
			if (arr[parent] > arr[max_pos]) {
				break;
			}
			int tmp;
			tmp = arr[parent];
			arr[parent] = max;
			arr[max_pos] = tmp;
			parent = max_pos;
		}
	}
	print_arr(arr, n);

	int len = n;
	while (len > 1) {
		int swp_tmp = arr[len - 1];
		arr[len - 1] = arr[0];
		arr[0] = swp_tmp;
		int parent = 0;
		while (1) {
			int max, max_pos;
			if (2 * (parent + 1) < len - 1) {
				if (arr[2 * parent + 1] > arr[2 * (parent + 1)]) {
					max_pos = 2 * parent + 1;
					max = arr[2 * parent + 1];
				} else {
					max_pos = 2 * (parent + 1);
					max = arr[2 * (parent + 1)];
				}
			} else if (2 * parent + 1 < len - 1) {
				max_pos = 2 * parent + 1;
				max = arr[2 * parent + 1];
			} else {
				break;
			}
			if (arr[parent] > arr[max_pos]) {
				break;
			}
			int tmp;
			tmp = arr[parent];
			arr[parent] = max;
			arr[max_pos] = tmp;
			parent = max_pos;
		}
		len--;
		print_arr(arr, n);
	}
}
