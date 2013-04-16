/* traverse.c */

/* traverse the expression tree to preorder ,inorder, postorder list.
 * (* (/ (- 74 10) 32)(+ 23 17)) = 80 preorder
 * (((74 - 10)/ 32) * (23 + 17)) = 80 inorder
 * (((74 10 -) 32 /) (23 17 +) *) = 80 postorder */

#include "../list.h"
#include "bitree.h"

/* preorder */
int preorder(const BiTreeNode *node, List *list) {
	/* Load the list with a preorder of the tree. */
	if (!bitree_is_eob(node)) {
		if (list_ins_next(list, list_tail(list), bitree_data(node)) != 0) {
			return -1;
		}
		if (!bitree_is_eob(bitree_left(node))) {
			if (preorder(bitree_left(node), list) != 0) {
				return -1;
			}
			if (!bitree_is_eob(bitree_right(node))) {
				if (preorder(bitree_right(node), list) != 0) {
					return -1;
				}
			}
		}
	}
	return 0;
}

/* inorder */
int inorder(const BiTreeNode *node, List *list) {
	/* Load the list with an inorder listing of the tree. */
	if (!bitree_is_eob(node)) {
		if (!bitree_is_eob(bitree_left(node))) {
			if (inorder(bitree_left(node), list) != 0) {
				return -1;
			}
		}
		if (list_ins_next(list, list_tail(list),bitree_data(node)) != 0) {
			return -1;
		}
		if (!bitree_is_eob(bitree_right(node))) {
			if (inorder(bitree_right(node), list) != 0) {
				return -1;
			}
		}
	}
	return 0;
}

/* postorder */
int postorder(const BiTreeNode *node, List *list) {
	/* Load the list with a postorder list of the tree. */
	if (!bitree_is_eob(node)) {
		if (!bitree_is_eob(bitree_left(node))) {
			if (postorder(bitree_left(node), list) != 0) {
				return -1;
			}
		}
		if (!bitree_is_eob(bitree_right(node))) {
			if (postorder(bitree_right(node), list) != 0) {
				return -1;
			}
		}
		if (list_ins_next(list, list_tail(list), bitree_data(node)) != 0) {
			return -1;
		}
	}
	return 0;
}
