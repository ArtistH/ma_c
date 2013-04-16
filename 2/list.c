/* list.c */
#include <stdlib.h>
#include <string.h>

#include "list.h"

/* list_init */
void list_init(List *list, void (*destroy)(void *data))
{
	/* Initialize the list */
	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;
	list->tail = NULL;

	return;
}

/* list_destroy */
void list_destroy(List *list)
{
	void *data;

	/* Remove each element. */
	while (list_size(list) > 0) {
		if (list_rem_next(list, NULL, (void *)&data) == 0
		    && list_destroy != NULL) {
			list_destroy(data);
		}
	}

	/* No operations are allowed now, but clear the structure
	 * as a precaution. */
	memset(list, 0, sizeof(List));

	return;
}

/* list_ins_next */
int list_ins_next(List *list, ListElmt *element, const void *data)
{
	ListElmt *new_element;

	/* Allocate storage for the element. */
	if ((new_element = (ListElmt *)malloc(sizeof(ListElmt))) == NULL) {
		return -1;
	}

	/* Insert the element into the list. */
	new_element->data = (void *)data;
	if (element == NULL) {
		/* Handle insertion at the head of the list.  */
		if (list_size(list) == 0) {
			list->tail = new_element;
		}

		new_element->next = list->head;
		list->head = new_element;
	} else {
		/* Handle insertion somewhere other than at the head.*/
		if (element->next == NULL) {
			list->tail = new_element;
		}

		new_element->next = element->next;
		element->next = new_element;
	}

	/* Adjust the size of list to account for the inserted element. */
	list->size++;

	return 0;
}
