/**
 * usage: quick_sort n
 * n is the number of the array that will be sorted, 10 by default
 * svn checkout http://exercise.googlecode.com/svn/trunk/ exercise-read-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int partition(int *arr, int first, int last)
{
	int i = first - 1, j = first;
	while (j < last - 1) {
		if (arr[j] < arr[last - 1]) {
			i++;
			swap(arr + i, arr + j);
		}
		j++;
	}
	i++;
	swap(arr + i, arr + last - 1);
	print_arr(arr, 10);
	return i;
}

void quick_sort(int *arr, int first, int last)
{
	if (last - 1 <= first)
		return;
	int rand_pos = rand() % (last - first) + first;
	swap(arr + rand_pos, arr + last - 1);

	int center = partition(arr, first, last);
	quick_sort(arr, first, center);
	quick_sort(arr, center + 1, last);
}

int main(int argc, char **argv)
{
	int n;
	if (argc < 2) {
		n = 10;
	} else {
		n = atoi(argv[1]);
	}
	int *arr = (int *)malloc(sizeof(int) * n);
	int i;
	srand(time(0));
	for (i = 0; i < n; i++) {
		arr[i] = rand() % 100;
	}
	printf("ini_array: ");
	print_arr(arr, n);
	quick_sort(arr, 0, n);
	printf("sorted_array: ");
	print_arr(arr, n);
	free(arr);
}
