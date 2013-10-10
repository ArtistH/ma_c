/* page.c */
#include "Cbasic/clist.h"
#include "Cbasic/page.h"

/* replace_page */
int replace_page(CListElmt **current) {
	/* Circle through the list of pages until one is fount to replace. */
	while (((Page *)(*current)->data)->reference != 0) {
		((Page *)(*current)->data)->reference = 0;
		*current = clist_next(*current);
	}

	return ((Page *)(*current)->data)->number;
}
