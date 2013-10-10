/* ex-1.c for open-addressed hash table. */
#include <stdio.h>
#include <string.h>
#include "Cbasic/ohtbl.h"

/* Define the size of the hash table. */
#define TBLSIZ 11

/* match_char */
static int match_char(const void *char1, const void *char2) {
	/* Determine whether two characters match. */
	return (*(const char *)char1 == *(const char *)char2);
}

/* h1_char */
static int h1_char(const void *key) {
	/* Define a simplistic auxilary hash function. */
	return *(const char *)key % TBLSIZ;
}

/* h2_char */
static int h2_char(const void *key) {
	/* Define a simplistic auxilary hash function. */
	return 1 + (*(const char *)key % (TBLSIZ - 2));
}

/* print_table */
static void print_table(const OHTbl *htbl) {
	int i = 0;

	/* Display the open-addressed hash table. */
	fprintf(stdout, "==> Table size is %d\n", ohtbl_size(htbl));
	for (i = 0; i < TBLSIZ; i++) {
		if (htbl->table[i] != NULL && htbl->table[i] != htbl->vacated) {
			fprintf(stdout, "\tSlot[%03d]=%c\n", i, *(char *)htbl->table[i]);
		} else {
			fprintf(stdout, "\tSlot[%03d]= \n", i);
		}
	}
	return;
}

/* local_malloc_insert */
static void local_malloc_insert(OHTbl *htbl, const char *string) {
	fprintf(stdout, "Inserting [%s]\t\t", string);
	char *data = NULL;
	if ((data = (char *)malloc(sizeof(char))) == NULL) {
		fprintf(stdout, "Malloc Error\n");
		return;
	}
	strncpy(data, string, sizeof(char));

	int retval = ohtbl_insert(htbl, data);
	if (retval == 0) {
		fprintf(stdout, "\n");
	} else if (retval == 1) {
		fprintf(stdout, "Already Exist\n");
		free(data);
	} else {
		fprintf(stdout, "Inserting Error\n");
		free(data);
	}

	return;
}

/* local_remove_free */
static void local_remove_free(OHTbl *htbl, const char *string) {
	fprintf(stdout, "Removing %s\t\t", string);
	char *data_str = NULL;
	if ((data_str = (char *)malloc(sizeof(char))) == NULL) {
		fprintf(stdout, "Malloc Error\n");
		return;
	}
	strncpy(data_str, string, sizeof(char));
	void *data = data_str;

	int retval = ohtbl_remove(htbl, (void **)&data);
	if (retval == 0) {
		fprintf(stdout, "\n");
		free(data);
	} else if (retval == -1) {
		fprintf(stdout, "Not found\n");
	}

	free(data_str);
	return;
}

/* local_malloc_insert_hash */
static void local_malloc_insert_hash(OHTbl *htbl,
									 int (*h1)(const void *key),
									 int (*h2)(const void *key),
									 const char *string) {
	local_malloc_insert(htbl, string);
	fprintf(stdout, "Hashing %s\n", string);
	int j = 0;
	for (; j < TBLSIZ; j++) {
		fprintf(stdout, " %02d", (h1(string) + (j * h2(string))) % TBLSIZ);
	}
	fprintf(stdout, "\n");
	return;
}

/* local_lookup */
static void local_lookup(OHTbl *htbl, const char *string) {
	fprintf(stdout, "Looking up '%s'\t\t", string);
	char *data_str = NULL;
	if ((data_str = (char *)malloc(sizeof(char))) == NULL) {
		fprintf(stdout, "Malloc Error\n");
		return;
	}
	strncpy(data_str, string, sizeof(char));
	void *data = data_str;

	int retval = ohtbl_lookup(htbl, (void **)&data);
	if (retval == 0) {
		fprintf(stdout, "Found\n");
	} else {
		fprintf(stdout, "Not Found\n");
	}

	free(data_str);
	return;
}

/* main */
int main() {
	OHTbl htbl;
	char *data = NULL;
	int i = 0;
	int j = 0;

	/* Initialize the open-addressed hash table. */
	if (ohtbl_init(&htbl, TBLSIZ, h1_char, h2_char, match_char, free) != 0) {
		return 1;
	}

	/* Perform some open-addressed hash table operations. */
	for (i = 0; i < 5; i++) {
		if ((data = (char *)malloc(sizeof(char))) == NULL) {
			return 1;
		}

		/* The following expression produces "random" data while avoiding
		 * duplicates. */
		*data = ((8 + (i * 9)) % 23) + 'A';
		fprintf(stdout, "Hashing %c:\n", *data);
		for (j = 0; j < TBLSIZ; j++) {
			fprintf(stdout, " %02d",
					(h1_char(data) + (j * h2_char(data))) % TBLSIZ);
		}
		fprintf(stdout, "\n");
		if (ohtbl_insert(&htbl, data) != 0) {
			return 1;
		}
		print_table(&htbl);
	}

	for (i = 0; i < 5; i++) {
		if ((data = (char *)malloc(sizeof(char))) == NULL) {
			return 1;
		}

		/* The following expression works similar to the one above but
		 * produces collisions. */
		*data = ((8 + (i * 9)) % 13) + 'j';
		fprintf(stdout, "Hashing %c\n", *data);
		for (j = 0; j < TBLSIZ; j++) {
			fprintf(stdout, " %02d",
					(h1_char(data) + (j * h2_char(data))) % TBLSIZ);
		}
		fprintf(stdout, "\n");
		if (ohtbl_insert(&htbl, data) != 0) {
			return 1;
		}
		print_table(&htbl);
	}

	local_malloc_insert(&htbl, "R");
	local_malloc_insert(&htbl, "R");
	local_malloc_insert(&htbl, "n");
	local_malloc_insert(&htbl, "o");

	local_remove_free(&htbl, "R");
	local_remove_free(&htbl, "n");
	local_remove_free(&htbl, "o");
	print_table(&htbl);

	local_malloc_insert_hash(&htbl, h1_char, h2_char, "R");
	local_malloc_insert_hash(&htbl, h1_char, h2_char, "n");
	local_malloc_insert_hash(&htbl, h1_char, h2_char, "o");
	print_table(&htbl);

	local_malloc_insert(&htbl, "X");
	local_malloc_insert(&htbl, "Y");
	print_table(&htbl);

	local_lookup(&htbl, "o");
	local_lookup(&htbl, "Z");

	/* Destroy the open-addressed hash table. */
	fprintf(stdout, "Destroying the hash table\n");
	ohtbl_destroy(&htbl);

	return 0;
}
