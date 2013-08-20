/* ex-2.c for cover.c (using set.c) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cover.h"
#include "list.h"
#include "set.h"

/* Define the number of numbers in the skills and player sets. */
#define SKILLS_COUNT 12
#define PLAYER_COUNT 8

/* print_skills */
static void print_skills(const Set *skills) {
	ListElmt *member = NULL;
	char *data = NULL;
	int size;
	int i;

	/* Display the set of skills. */
	fprintf(stdout, "Set size is %d\n", size = set_size(skills));
	i = 0;
	member = list_head(skills);
	while (i < size) {
		data = list_data(member);
		fprintf(stdout, "skills[%03d]=%c\n", i, *data);
		i++;
	}
	return;
}

/* print_players */
static void print_players(const Set *players) {
	ListElmt *member = NULL;

	/* Display the skills for each player in the set of players. */
	for (member = list_head(players);
		 member != NULL;
		 member = list_next(member)) {
		fprintf(stdout, "Player %c\n",
				*(char *)((KSet *)list_data(member))->key);
		print_skills(&((KSet *)list_data(member))->set);
	}
}

/* match_key */
int match_key(const void *key1, const void *key2) {
	/* Determine whether two keyed sets match. */
	if (*(char *)((const KSet *)key1)->key
		== *(char *)((const KSet *)key2)->key) {
		return 1;
	} else {
		return 0;
	}
}

/* match_skill */
int match_skill(const void *skill1, const void *skill2) {
	/* Determine whether two skills match. */
	if (*(const char *)skill1 == *(const char *)skill2) {
		return 1;
	} else {
		return 0;
	}
}

/* main */
int main() {
	return 0;
}
