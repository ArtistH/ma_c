/**
 * usage: HeapSort n
 * n is the number of the array that will be sorted, 10 by default
 * svn checkout http://exercise.googlecode.com/svn/trunk/ exercise-read-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUM 1000

typedef struct _node {
	int key;
	int value;
} Node;

void print_arr(Node * arr, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (!i) {
			printf("%d", arr[i].value);
		} else {
			printf(" %d", arr[i].value);
		}
	}
	printf("\n");
}

void print_arr_key(Node * arr, int n)
{
	int i;
	for (i = 0; i < n; i++) {
		if (!i) {
			printf("%d", arr[i].key);
		} else {
			printf(" %d", arr[i].key);
		}
	}
	printf("\n");
}

void counting_sort(Node * ini_arr, Node * sorted_arr, int n)
{
	int *count_arr = (int *)malloc(sizeof(int) * 16);
	int i, j, k;
	for (k = 0; k < 16; k++) {
		count_arr[k] = 0;
	}
	for (i = 0; i < n; i++) {
		count_arr[ini_arr[i].key]++;
	}
	for (k = 1; k < 16; k++) {
		count_arr[k] += count_arr[k - 1];
	}
	for (j = n; j > 0; j--) {
		sorted_arr[count_arr[ini_arr[j - 1].key] - 1] = ini_arr[j - 1];
		count_arr[ini_arr[j - 1].key]--;
	}
	free(count_arr);
}

void radix_sort(Node * ini_arr, Node * sorted_arr, int n)
{
	int loop = 0;
	int t = MAX_NUM;
	while (t > 0) {
		loop++;
		t = t >> 4;
	}
	int cur_loop = 0;
	while (cur_loop < loop) {
		printf("cur_loop: %d\n", cur_loop);
		int i;
		for (i = 0; i < n; i++) {
			ini_arr[i].key =
			    ini_arr[i].value >> 4 * cur_loop & 0x0F;
		}
		print_arr_key(ini_arr, n);
		counting_sort(ini_arr, sorted_arr, n);
		print_arr_key(sorted_arr, n);
		for (i = 0; i < n; i++) {
			ini_arr[i].value = sorted_arr[i].value;
		}
		cur_loop++;
	}
}

int main(int argc, char **argv)
{
	int n;
	if (argc < 2) {
		n = 10;
	} else {
		n = atoi(argv[1]);
	}
	int i;
	Node *ini_arr = (Node *) malloc(sizeof(Node) * n);
	Node *sorted_arr = (Node *) malloc(sizeof(Node) * n);
	srand(time(0));
	for (i = 0; i < n; i++) {
		ini_arr[i].value = rand() % MAX_NUM;
		ini_arr[i].key = ini_arr[i].value;
	}
	printf("ini_array: ");
	print_arr(ini_arr, n);
	radix_sort(ini_arr, sorted_arr, n);
	printf("sorted_array: ");
	print_arr(sorted_arr, n);
	free(ini_arr);
	free(sorted_arr);
}
