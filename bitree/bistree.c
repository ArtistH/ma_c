/* bistree.c */
#include <stdlib.h>
#include <string.h>
#include "bistree.h"

static void destroy_right(BisTree *tree, BiTreeNode *node);

/* rotate_left */
static void rotate_left(BiTreeNode **node) {
	BiTreeNode *left, *grandchild;

	left = bitree_left(*node);
	if (((AvlNode *)bitree_data(left))->factor == AVL_LFT_HEAVY) {
		/* Perform an LL rotation. */
		bitree_left(*node) = bitree_right(left);
		bitree_right(left) = *node;
		((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
		*node = left;
	}
	else {
		/* Perform an LR rotation. */
		grandchild = bitree_right(left);
		bitree_right(left) = bitree_left(grandchild);
		bitree_left(grandchild) = left;
		bitree_left(*node) = bitree_right(grandchild);
		bitree_right(grandchild) = *node;

		switch (((AvlNode *)bitree_data(grandchild))->factor) {
		case AVL_LFT_HEAVY:
			((AvlNode *)bitree_data(*node))->factor = AVL_RGT_HEAVY;
			((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
			break;
		case AVL_BALANCED:
			((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *)bitree_data(left))->factor = AVL_BALANCED;
			break;
		case AVL_RGT_HEAVY:
			((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *)bitree_data(left))->factor = AVL_LFT_HEAVY;
			break;
		}

		((AvlNode *)bitree_data(grandchild))->factor = AVL_BALANCED;
		*node = grandchild;
	}
	return;
}

/* rotate_right */
static void rotate_right(BiTreeNode **node) {
	BiTreeNode *right, *grandchild;
	right = bitree_data(*node);

	if (((AvlNode *)bitree_data(right))->factor == AVL_RGT_HEAVY) {
		/* Perform an RR rotation. */
		bitree_right(*node) = bitree_left(right);
		bitree_left(right) = *node;
		((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
		((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
		*node = right;
	}
	else {
		/* Perform an RL rotation. */
		grandchild = bitree_left(right);
		bitree_left(right) = bitree_right(grandchild);
		bitree_right(grandchild) = right;
		bitree_right(*node) = bitree_left(grandchild);
		bitree_left(grandchild) = *node;

		switch (((AvlNode *)bitree_data(grandchild))->factor) {
		case AVL_LFT_HEAVY:
			((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *)bitree_data(right))->factor = AVL_RGT_HEAVY;
			break;
		case AVL_BALANCED:
			((AvlNode *)bitree_data(*node))->factor = AVL_BALANCED;
			((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
			break;
		case AVL_RGT_HEAVY:
			((AvlNode *)bitree_data(*node))->factor = AVL_LFT_HEAVY;
			((AvlNode *)bitree_data(right))->factor = AVL_BALANCED;
			break;
		}
		((AvlNode *)bitree_data(grandchild))->factor = AVL_BALANCED;
		*node = grandchild;
	}
	return;
}
