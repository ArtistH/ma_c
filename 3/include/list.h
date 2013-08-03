#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/* Define a structure for linked list elements. */
typedef struct ListElmt_ {
	void *data;
	struct ListElmt_ *next;
}ListElmt;

/* Define a structure for linked list. */
typedef struct List_ {
	int size;
	int (*match)(const void *key1, const void *key2);
	void (*destroy)(void *data);
	ListElmt *head;
	ListElmt *tail;
}List;
