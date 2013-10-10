/* ex-1.c for bistree (using AVL) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cbasic/bistree.h"

/* Define the size of strings. */
#define STRSIZ 6

/* preorder_tree */
static void preorder_tree(const BiTreeNode *node) {
	/* Display the binary search tree rooted at the specified node. */
	if (!bitree_is_eob(node)) {
		fprintf(stdout, "Node=%s, %+2d, hidden=%d\n",
				(char *)((AvlNode *)bitree_data(node))->data,
				((AvlNode *)bitree_data(node))->factor,
				((AvlNode *)bitree_data(node))->hidden);

		if (!bitree_is_eob(bitree_left(node))) {
			preorder_tree(bitree_left(node));
		}

		if (!bitree_is_eob(bitree_right(node))) {
			preorder_tree(bitree_right(node));
		}
	}

	return;
}

/* compare_str */
static int compare_str(const void *str1, const void *str2) {
	int retval;

	/* Compare two strings. */
	if ((retval = strcmp((const char *)str1, (const char *)str2)) > 0) {
		return 1;
	} else if (retval < 0) {
		return -1;
	} else {
		return 0;
	}
}

/* print_all_nodes */
static void print_all_nodes(BiTree *tree) {
	fprintf(stdout, "Tree size is %d\n", bistree_size(tree));
	fprintf(stdout, "(Preorder traversal)\n");
	preorder_tree(bitree_root(tree));
	fprintf(stdout, "\n");
}

/* local_tree_node_insert */
static void local_tree_node_insert(BiTree *tree, void *data) {
	fprintf(stdout, "Inserting %s\n", (char *)data);
	if (bistree_insert(tree, data) == 1) {
		fprintf(stdout, "Node(%s) had existed\n\n", (char *)data);
	}
	print_all_nodes(tree);
}

/* local_tree_node_remove */
static void local_tree_node_remove(BiTree *tree, void *data) {
	fprintf(stdout, "Removing %s\n", (char *)data);
	if (bistree_remove(tree, (char *)data) != 0) {
		fprintf(stdout, "Could not find %s\n", (char *)data);
	} else {
		print_all_nodes(tree);
	}
}

/* local_tree_node_lookup */
static void local_tree_node_lookup(BiTree *tree, void *data) {
	if (bistree_lookup(tree, &data) == -1) {
		fprintf(stdout, "Could not find %s\n", (char *)data);
	} else {
		fprintf(stdout, "Found %s\n", (char *)data);
	}
}

/* Define an enum for various words to search. */
typedef enum Words_ {hop, hat, tap, bat, tip, mop, mom, cat, zoo, wax,
	top, dip} Words;

/* main */
int main() {
	BiTree tree;
	char sarray[12][STRSIZ];
	char tarray[12][STRSIZ];

	/* Load an array with the data to search. */
	strcpy(sarray[hop], "hop");
	strcpy(sarray[hat], "hat");
	strcpy(sarray[tap], "tap");
	strcpy(sarray[bat], "bat");
	strcpy(sarray[tip], "tip");
	strcpy(sarray[mop], "mop");
	strcpy(sarray[mom], "mom");
	strcpy(sarray[cat], "cat");
	strcpy(sarray[zoo], "zoo");
	strcpy(sarray[wax], "wax");
	strcpy(sarray[top], "top");
	strcpy(sarray[dip], "dip");

	/* Initialize the binary search tree. */
	bistree_init(&tree, compare_str, NULL);

	/* Perform some binary search tree operations. */
	fprintf(stdout, "Inserting some nodes\n");

	local_tree_node_insert(&tree, sarray[tap]);
	local_tree_node_insert(&tree, sarray[tip]);
	local_tree_node_insert(&tree, sarray[top]);
	local_tree_node_insert(&tree, sarray[cat]);
	local_tree_node_insert(&tree, sarray[bat]);

	local_tree_node_remove(&tree, sarray[tap]);
	local_tree_node_remove(&tree, sarray[tip]);
	local_tree_node_remove(&tree, sarray[hop]);

	local_tree_node_insert(&tree, sarray[dip]);
	local_tree_node_insert(&tree, sarray[tap]);
	local_tree_node_insert(&tree, sarray[top]);
	local_tree_node_insert(&tree, sarray[tip]);

	fprintf(stdout, "Inserting more nodes\n");

	local_tree_node_insert(&tree, sarray[mom]);
	local_tree_node_insert(&tree, sarray[hat]);
	local_tree_node_insert(&tree, sarray[mop]);
	local_tree_node_insert(&tree, sarray[wax]);
	local_tree_node_insert(&tree, sarray[zoo]);

	local_tree_node_remove(&tree, sarray[wax]);
	local_tree_node_remove(&tree, sarray[hop]);

	fprintf(stdout, "Looking up some nodes\n");

	strcpy(tarray[0], "top");
	strcpy(tarray[1], "hop");
	strcpy(tarray[2], "wax");
	strcpy(tarray[3], "hat");
	strcpy(tarray[4], "xxx");

	local_tree_node_lookup(&tree, tarray[0]);
	local_tree_node_lookup(&tree, tarray[1]);
	local_tree_node_lookup(&tree, tarray[2]);
	local_tree_node_lookup(&tree, tarray[3]);
	local_tree_node_lookup(&tree, tarray[4]);

	/* Destroy the binary search tree. */
	fprintf(stdout, "Destroying the search tree\n");
	bistree_destroy(&tree);

	return 0;
}
