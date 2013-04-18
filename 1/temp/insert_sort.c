/**
 * usage: insert_sort n
 * n is the number of the array that will be sorted, 10 by default
 * svn checkout http://exercise.googlecode.com/svn/trunk/ exercise-read-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_arr(int *arr, int n);
void insert_sort(int *arr, int n);

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
	insert_sort(arr, n);
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

void insert_sort(int *arr, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		int flag = arr[i];
		int j;
		for (j = i; j > 0; j--) {
			if (arr[j - 1] > flag) {
				arr[j] = arr[j - 1];
			} else {
				arr[j - 1] = flag;
				break;
			}
			arr[j - 1] = flag;
		}
	}
}
