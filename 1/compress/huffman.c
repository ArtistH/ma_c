/* huffman.c */
#include <limits.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

#include "bit.h"
#include "compress.h"
#include "../bitree/bitree.h"
#include "../heap/pqueue.h"

/* compare_freq */
static int compare_freq(const void *tree1, const void *tree2)
{
	HuffNode *root1, *root2;

	/* Compare the frequencies stored in the root nodes of
	 * two binary tree. */
	root1 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree1));
	root2 = (HuffNode *)bitree_data(bitree_root((const BiTree *)tree2));

	if (root1->freq < root2->freq) {
		return 1;
	} else if (root1->freq > root2->freq) {
		return -1;
	} else {
		return 0;
	}
}

/* destroy_tree */
static void destroy_tree(void *tree)
{
	/* Destroy and free one binary from the 
		* priority queue of trees */
	bitree_destory(tree);
	free(tree);

	return;
}

/* build_tree */
static int build_tree(int *freqs, BiTree **tree)
{
	BiTree *init, *merge, *left, *right;
	PQueue pqueue;
	HuffNode *data;
	int size, c;

	/* Initialize the priority queue of binary trees. */
	*tree = NULL;

	pqueue_init(&pqueue, compare_freq, destroy_tree);
	for (c = 0; c < UCHAR_MAX; c++) {
		if (freqs[c] != 0) {
			/* Set up a binary_tree for the current symbol
			 * and its frequency. */
			if ((init = (BiTree *)malloc(sizeof(BiTree))) == NULL) {
				pqueue_destroy(&pqueue);
				return -1;
			}

			bitree_init(init, free);
			if ((data = (HuffNode *)malloc(sizeof(HuffNode))) 
			    == NULL) {
				pqueue_destroy(&pqueue);
				return -1;
			}

			data->symbol = c;
			data->freq = freqs[c];

			if (bitree_ins_left(init, NULL, data) != 0) {
				free(data);
				bitree_destory(init);
				free(init);
				pqueue_destroy(&pqueue);
				return -1;
			}

			/* Insert the binary tree into the priority queue. */
			if (pqueue_insert(&pqueue, init) != 0) {
				bitree_destory(init);
				free(init);
				pqueue_destroy(&pqueue);
				return -1;
			}
		}
	}

	/* Build a Huffman tree by merging trees in the priority queue. */
	size = pqueue_size(&pqueue);

	for (c = 1; c <= size - 1; c++) {
		/* Allocate storage for the next merged tree. */
		if ((merge = (BiTree *)malloc(sizeof(BiTree))) == NULL) {
			pqueue_destroy(&pqueue);
			return -1;
		}

		/* Extract the two trees whose root nodes have 
		 * the smallest frequencies. */
		if (pqueue_extract(&pqueue, (void **)&left) != 0) {
			pqueue_destroy(&pqueue);
			free(merge);
			return -1;
		}

		if (pqueue_extract(&pqueue, (void **)&right) != 0) {
			pqueue_destroy(&pqueue);
			free(merge);
			return -1;
		}

		/* Allocate the storage the data in the root node of
		 * the merged tree.  */
		if ((data = (HuffNode *)malloc(sizeof(HuffNode))) == NULL) {
			pqueue_destroy(&pqueue);
			free(merge);
			return -1;
		}

		memset(data, 0, sizeof(HuffNode));

		/* Sum the frequencies in the root nodes of the trees
		 * being merged. */
		data->freq = ((HuffNode *)bitree_data(bitree_root(left)))->freq
			+ ((HuffNode *)bitree_data(bitree_root(left)))->freq;

		/* Merge the two trees. */
		if (bitree_merge(merge, left, right, data) != 0) {
			pqueue_destroy(&pqueue);
			free(merge);
			return -1;
		}

		/* Insert the merged tree into the priority queue and 
		 * free the others. */
		if (pqueue_insert(&pqueue, merge) != 0) {
			pqueue_destroy(&pqueue);
			bitree_destory(merge);
			free(merge);
			return -1;
		}

		free(left);
		free(right);
	}

	/* The last tree in the priority queue is the Huffman tree. */
	if (pqueue_extract(&pqueue, (void **)tree) != 0) {
		pqueue_destroy(&pqueue);
		return -1;
	} else {
		pqueue_destroy(&pqueue);
	}
	return 0;
}
