/* ex-1.c for chtbl (chained hash table) */
#include <stdio.h>
#include "Cbasic/chtbl.h"
#include "Cbasic/list.h"

/* Define the size of the chained hash table. */
#define TBLSIZ 11

/* match_char */
static int match_char(const void *char1, const void *char2) {
	/* Compare two characters. */
	return (*(const char *)char1 == *(const char *)char2);
}

/* h_char */
static int h_char(const void *key) {
	/* Define a simplistic hash function. */
	return *(const char *)key % TBLSIZ;
}

static void print_table(const CHTbl *htbl) {
	ListElmt *element;
	int i;

	/* Display the chained hash table. */
	fprintf(stdout, "Table size is %d\n", chtbl_size(htbl));
	for (i = 0; i < TBLSIZ; i++) {
		fprintf(stdout, "Bucket[%03d]=", i);
		for (element = list_head(&htbl->table[i]);
			 element != NULL;
			 element = list_next(element)) {
			fprintf(stdout, "%c", *(char *)list_data(element));
		}
		fprintf(stdout, "\n");
	}

	fprintf(stdout, "\n");
	return;
}

/* local_htbl_node_insert */
static void local_htbl_node_insert(CHTbl *htbl, char string) {
	int retval;
	char *data = NULL;
	if ((data = (char *)malloc(sizeof(char) + 1)) == NULL) {
		fprintf(stdout, "Malloc Error\n");
	}
	char string_backup[2];
	data[0] = string_backup[0] = string;
	data[1] = string_backup[1] = '\0';
	if ((retval = chtbl_insert(htbl, data)) != 0) {
		free(data);
	}

	if (0 == retval) {
		fprintf(stdout, "Trying to insert %s ... SUCCESS\n", &string_backup[0]);
	} else if (1 == retval) {
		fprintf(stdout, "Trying to insert %s ... EXIST\n", &string_backup[0]);
	} else {
		fprintf(stdout, "Trying to insert %s ... ERROR\n", &string_backup[0]);
	}
}

/* local_htbl_node_remove */
static void local_htbl_node_remove(CHTbl *htbl, char string) {
	int retval;
	char string_backup[2];
	string_backup[0] = string;
	string_backup[1] = '\0';

	void *data = NULL;
	data = &string_backup[0];
	retval = chtbl_remove(htbl, (void **)&data);
	free(data);

	if (0 == retval) {
		fprintf(stdout, "Trying to remove %s ... SUCCESS\n", &string_backup[0]);
	} else if (1 == retval) {
		fprintf(stdout, "Trying to remove %s ... EXIST but ERROR\n", &string_backup[0]);
	} else {
		fprintf(stdout, "Trying to remove %s ... Not Found\n", &string_backup[0]);
	}
}

/* local_htbl_node_lookup */
static void local_htbl_node_lookup(CHTbl *htbl, char string) {
	int retval;
	char string_backup[2];
	string_backup[0] = string;
	string_backup[1] = '\0';

	void *data = NULL;
	data = &string_backup[0];
	retval = chtbl_lookup(htbl, (void **)&data);

	if (0 == retval) {
		fprintf(stdout, "Found %s ... SUCCESS\n", &string_backup[0]);
	} else {
		fprintf(stdout, "Not Found %s ... FAILURE\n", &string_backup[0]);
	}
}


int main() {
	CHTbl htbl;
	char *data;
	int i;

	/* Initialize the chained hash table. */
	if (chtbl_init(&htbl, TBLSIZ, h_char, match_char, free) != 0) {
		return 1;
	}

	/* Perform some chained hash table operations. */
	for (i = 0; i < TBLSIZ; i++) {
		if ((data = (char *)malloc(sizeof(char))) == NULL) {
			return 1;
		}

		*data = ((5 + (i * 6)) % 23) + 'A';

		if (chtbl_insert(&htbl, data) != 0) {
			return 1;
		}
	}
	print_table(&htbl);

	for (i = 0; i < TBLSIZ; i++) {
		if ((data = (char *)malloc(sizeof(char))) == NULL) {
			return 1;
		}

		*data = ((3 + (i * 4)) % 23) + 'a';

		if (chtbl_insert(&htbl, data) != 0) {
			return 1;
		}
	}
	print_table(&htbl);

	local_htbl_node_insert(&htbl, 'd');
	local_htbl_node_insert(&htbl, 'G');

	local_htbl_node_remove(&htbl, 'd');
	local_htbl_node_remove(&htbl, 'G');
	local_htbl_node_remove(&htbl, 'S');

	print_table(&htbl);

	local_htbl_node_insert(&htbl, 'd');
	local_htbl_node_insert(&htbl, 'G');

	print_table(&htbl);

	fprintf(stdout, "Inserting X and Y\n");

	local_htbl_node_insert(&htbl, 'X');
	local_htbl_node_insert(&htbl, 'Y');

	print_table(&htbl);

	local_htbl_node_lookup(&htbl, 'X');
	local_htbl_node_lookup(&htbl, 'Z');

	/* Destroy the chained hash table. */
	fprintf(stdout, "Destroying the hash table\n");
	chtbl_destroy(&htbl);

	return 0;
}
