/* spell.c */
/* O(m lg n)
 * m the number of the words in dictionary. 
 * n the words what to check */
#include <string.h>

#include "search.h"
#include "spell.h"

/* compare_str */
static int compare_str(const void *str1, const void *str2)
{
	int retval;

	if ((retval = strcmp((const char *)str1, (const char *)str2)) > 0) {
		return 1;
	} else if (retval < 0) {
		return -1;
	} else {
		return 0;
	}
}

/* spell */
/* O(lg n)
 * like bisearch 
 * n the number of words in dictionary */
int spell(char (*dictionary)[SPELL_SIZE], int size, const char *word)
{
	/* Look up the word. */
	if (bisearch(dictionary, word, size, SPELL_SIZE, compare_str) >= 0) {
		return 1;
	} else {
		return 0;
	}
}
