/* ex-1.c for set */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "set.h"

/* print_set */
static void print_set(const Set *set) {
	ListElmt *member = NULL;
	int *data = NULL;
	int size = 0;
	int i = 0;

	/* Display the set. */
	fprintf(stdout, "Set size is %d\n", size = set_size(set));
	i = 0;
	member = list_head(set);
	while (i < size) {
		data = list_data(member);
		fprintf(stdout, "set[%03d]=%03d\n", i, *data);
		member = list_next(member);
		i++;
	}
	fprintf(stdout, "\n");

	return;
}

/* match_int */
static int match_int(const void *key1, const void *key2) {
	/* Determine whether two integers match. */
	return !memcmp(key1, key2, sizeof(int));
}

/* local_set_insert_int */
static void local_set_insert_int(Set *set, int number) {
	int *data = NULL;
	if ((data = (int *)malloc(sizeof(int))) == NULL) {
		fprintf(stdout, "Malloc Error\n");
		return;
	}

	*data = number;

	int retval = set_insert(set, data);
	if (retval < 0) {
		fprintf(stdout, "During %4d Inserting, Malloc Error\n", *data);
		return;
	} else if (retval == 1) {
		fprintf(stdout, "During %4d Inserting, Already EXIST\n", *data);
		free(data);
	} else if (retval == 0) {
		fprintf(stdout, "During %4d Inserting, SUCCESS \n", *data);
	}
}

/* local_set_remove_int */
static void local_set_remove_int(Set *set, int number) {
	int number_backup = number;
	int *data = &number_backup;

	int retval = set_remove(set, (void **)&data);
	if (retval == 0) {
		free(data);
		fprintf(stdout, "During %4d Removing, SUCCESS\n", number_backup);
	} else if (retval == -1) {
		fprintf(stdout, "During %4d Removing, Not Found\n", number_backup);
	}
}

/* main */
int main() {
	Set set;
	Set set1;
	Set set2;
	int *data = NULL;
	int i = 0;

	/* Initialize the set. */
	set_init(&set, match_int, free);

	/* Perform some set operations. */
	fprintf(stdout, "Inserting 10 members\n");
	for (i = 10; i > 0; i--) {
		local_set_insert_int(&set, i);
	}
	print_set(&set);

	fprintf(stdout, "Inserting the same members again\n");
	for (i = 10; i > 0; i--) {
		local_set_insert_int(&set, i);
	}
	print_set(&set);

	local_set_insert_int(&set, 200);
	local_set_insert_int(&set, 100);

	print_set(&set);
	fprintf(stdout, "Removing 100, 5, and 10\n");

	local_set_remove_int(&set, 100);
	local_set_remove_int(&set, 100);
	local_set_remove_int(&set, 5);
	local_set_remove_int(&set, 10);

	print_set(&set);
	fprintf(stdout, "Removing three members\n");

	if (list_rem_next(&set, NULL, (void **)&data) == 0) {
		free(data);
	}

	if (list_rem_next(&set, NULL, (void **)&data) == 0) {
		free(data);
	}

	if (list_rem_next(&set, NULL, (void **)&data) == 0) {
		free(data);
	}

	print_set(&set);
	fprintf(stdout, "Removing all members\n");

	while (set_size(&set) > 0) {
		if (list_rem_next(&set, NULL, (void **)&data) == 0) {
			free(data);
		}
	}

	print_set(&set);

	/* Initialize two sets. */
	set_init(&set1, match_int, free);
	set_init(&set2, match_int, free);

	/* Perform soem set operations. */
	for (i = 10; i > 0; i--) {
		local_set_insert_int(&set1, i);

		if (i == 5 || i == 6 || i == 7) {
			local_set_insert_int(&set2, i * 10);
		} else if (i == 3 || i == 1) {
			local_set_insert_int(&set2, i);
		}
	}

	fprintf(stdout, "Set 1 for testing unions, intersections, and differences\n");
	print_set(&set1);
	fprintf(stdout, "Set 2 for testing unions, intersections, and differences\n");
	print_set(&set2);

	fprintf(stdout, "Determining the union of the two sets\n");
	if (set_union(&set, &set1, &set2) != 0) {
		return 1;
	}
	print_set(&set);
	fprintf(stdout, "Destroying the union\n");
	set_destroy(&set);

	fprintf(stdout, "Determining the intersection of the two sets\n");

	if (set_intersection(&set, &set1, &set2) != 0) {
		return 1;
	}
	print_set(&set);

	fprintf(stdout, "Testing whether the intersection equals Set 1...Value=%d"
			" (0=OK)\n", set_is_equal(&set, &set1));

	fprintf(stdout, "Testing whether Set 1 equals itself...Value=%d (1=OK)\n",
			set_is_equal(&set1, &set1));

	fprintf(stdout, "Testing whether the intersection is a subset of Set 1..."
			"Value=%d (1=OK)\n", set_is_subset(&set, &set1));

	fprintf(stdout, "Testing whether Set 2 is a subset of Set 1...Value=%d "
			"(0=OK)\n", set_is_subset(&set2, &set1));

	fprintf(stdout, "Destroying the intersection\n");
	set_destroy(&set);

	fprintf(stdout, "Determining the difference of the two sets\n");
	if (set_difference(&set, &set1, &set2) != 0) {
		return 1;
	}
	print_set(&set);

	i = 3;
	fprintf(stdout, "Testing whether %03d is in difference...Value=%d (0=OK)\n",
			i, set_is_member(&set, &i));

	i = 5;
	fprintf(stdout, "Testing whether %03d is in difference...Value=%d (1=OK)\n",
			i, set_is_member(&set, &i));

	/* Destroy the sets. */
	fprintf(stdout, "Destroying the sets\n");
	set_destroy(&set);
	set_destroy(&set1);
	set_destroy(&set2);

	return 0;
}
