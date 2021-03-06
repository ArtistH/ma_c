/* route.c */
#include <stdlib.h>
#include "Cbasic/graphalg.h"
#include "Cbasic/list.h"
#include "Cbasic/route.h"

/* route */
int route(List *paths, PathVertex *destination, PathVertex **next,
		  int (*match)(const void *key1, const void *key2)) {
	PathVertex *temp = NULL;
	PathVertex *parent = NULL;
	ListElmt *element = NULL;
	int found;

	/* Locate the destination in the list of grateways. */
	found = 0;
	
	for (element = list_head(paths);
		 element != NULL;
		 element = list_next(element)) {
		if (match(list_data(element), destination)) {
			temp = list_data(element);
			parent = ((PathVertex *)list_data(element))->parent;
			found = 1;
			break;
		}
	}

	/* Return if the destination is not reachable. */
	if (!found) {
		return -1;
	}

	/* Compute the next gateway in the shortest path to the destination. */
	while (parent != NULL) {
		temp = list_data(element);
		found = 0;
		for (element = list_head(paths);
			 element != NULL;
			 element = list_next(element)) {
			if (match(list_data(element), parent)) {
				parent = ((PathVertex *)list_data(element))->parent;
				found = 1;
				break;
			}
		}

		/* Return if the destination is not reachable. */
		if (!found) {
			return -1;
		}
	}
	*next = temp;

	return 0;
}
