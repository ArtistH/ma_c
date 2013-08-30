/* ex-1.c using a graph */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "set.h"

/* Define the size of strings. */
#define STRSIZ 2

/* print_graph */
static void print_graph(const Graph *graph) {
	Set *adjacent = NULL;
	ListElmt *element = NULL;
	ListElmt *member = NULL;
	int i;
	int j;

	/* Display the graph. */
	fprintf(stdout, "==> Vertices=%d, edges=%d\n",
			graph_vcount(graph), graph_ecount(graph));

	for (i = 0, element = list_head(&graph_adjlists(graph));
		 i < list_size(&graph_adjlists(graph));
		 i++, element = list_next(element)) {

		fprintf(stdout, "\tgraph[%03d]=%s: ", i,
				(char *)((AdjList *)list_data(element))->vertex);

		adjacent = &((AdjList *)list_data(element))->adjacent;
		for (j = 0, member = list_head(adjacent);
			 j < set_size(adjacent);
			 j++, member = list_next(member)) {

			if (j > 0) {
				fprintf(stdout, ", ");
			}
			fprintf(stdout, "%s", (char *)list_data(member));
		}

		fprintf(stdout, "\n");
	}

	return;
}

/* match_str */
static int match_str(const void *str1, const void *str2) {
	/* Determine whether two strings match. */
	return !strcmp((const char *)str1, (const char *)str2);
}

/* local_malloc_insert_vertex */
static void local_malloc_insert_vertex(Graph *graph, char *string) {
	fprintf(stdout, "Inserting vertex %s", string);
	char *data = NULL;
	if ((data = (char *)malloc(STRSIZ)) == NULL) {
		fprintf(stdout, "\t\t --- [ERROR] : malloc error\n");
		return;
	}
	if (strlen(string) < STRSIZ) {
		strcpy(data, string);
	}

	int retval = graph_ins_vertex(graph, data);
	if (retval == 0) {
		fprintf(stdout, "\n");
		return;
	} else if (retval == -1) {
		free(data);
		fprintf(stdout, "\t\t --- [ERROR] : malloc error\n");
	} else if (retval == 1) {
		free(data);
		fprintf(stdout, "\t\t --- [ERROR] : vertex exist\n");
	}

	return;
}

/* local_malloc_insert_edge */
static void local_malloc_insert_edge(Graph *graph,
									 char *string1, char *string2) {
	fprintf(stdout, "Inserting edge %s --> %s", string1, string2);

	char data1[STRSIZ];
	if (strlen(string1) < STRSIZ) {
		strcpy(data1, string1);
	}

	char *data2 = NULL;
	if ((data2 = (char *)malloc(STRSIZ)) == NULL) {
		fprintf(stdout, "\t\t --- [ERROR] : malloc error\n");
		return;
	}
	if (strlen(string2) < STRSIZ) {
		strcpy(data2, string2);
	}

	int retval = graph_ins_edge(graph, data1, data2);

	if (retval == 0) {
		fprintf(stdout, "\n");
		return;
	} else if (retval == -1) {
		free(data2);
		fprintf(stdout, "\t\t --- [ERROR] : vertex not exist\n");
	} else if (retval == 1) {
		free(data2);
		fprintf(stdout, "\t\t --- [ERROR] : edge already exist\n");
	}

	return;
}

static void local_remove_edge(Graph *graph, char *string1, char *string2) {
	fprintf(stdout, "Removing edge %s --> %s", string1, string2);

	char data1[STRSIZ];
	if (strlen(string1) < STRSIZ) {
		strcpy(data1, string1);
	}

	char data2[STRSIZ];
	if (strlen(string2) < STRSIZ) {
		strcpy(data2, string2);
	}

	char *data = data2;
	int retval = graph_rem_edge(graph, data1, (void **)&data);

	if (retval == 0) {
		free(data);
		fprintf(stdout, "\n");
		return;
	} else if (retval == -1) {
		fprintf(stdout, "\t\t --- [ERROR] : vertex not exist\n");
	}

	return;
}

static void local_remove_vertex(Graph *graph, char *string) {
	char data1[STRSIZ];
	if (strlen(string) < STRSIZ) {
		strcpy(data1, string);
	}
	fprintf(stdout, "Removing vertex %s", data1);
	char *data = data1;
	int retval = graph_rem_vertex(graph, (void **)&data);

	if (retval == 0) {
		free(data);
		fprintf(stdout, "\n");
		return;
	} else if (retval == -1) {
		fprintf(stdout, "\t\t --- [ERROR] : vertex in adjacent list or not exist\n");
		return;
	}

	return;
}

static void local_check_adjacent(Graph *graph, char *string1, char *string2) {
	fprintf(stdout, "Testing adjacent (%s --> %s) = ", string1, string2);
	char data1[STRSIZ];
	if (strlen(string1) < STRSIZ) {
		strcpy(data1, string1);
	}
	char data2[STRSIZ];
	if (strlen(string2) < STRSIZ) {
		strcpy(data2, string2);
	}

	int retval = graph_is_adjacent(graph, data1, data2);
	if (retval == 1) {
		fprintf(stdout, "is adjacent\n");
	} else if (retval == 0) {
		fprintf(stdout, "isn't adjacent\n");
	} else if (retval == -1) {
		fprintf(stdout, " \t --- [ERROR] : vertex not found\n");
	}
	return;
}

/* local_checkout_adjlist */
static void local_check_adjlist(Graph *graph, char *string) {
	char data[STRSIZ];
	if (strlen(string) < STRSIZ) {
		strcpy(data, string);
	}

	AdjList *adjlist = NULL;
	int retval = graph_adjlist(graph, data, &adjlist);
	if (retval == 0) {
		fprintf(stdout, "Vertices adjacent to [%s] :  ", string);
	} else {
		fprintf(stdout, "Vertices [%s] not found\n", data);
		return;
	}

	/* Output the elements in AdjList */
	ListElmt* element = NULL;
	int i = 0;
	int size = set_size(&adjlist->adjacent);
	for (element = list_head(&adjlist->adjacent);
		 i < size;
		 i++, element = list_next(element)) {
		if (i > 0) {
			fprintf(stdout, ", ");
		}
		fprintf(stdout, "%s", (char *)list_data(element));
	}
	fprintf(stdout, "\n");

	return;
}


/* main */
int main() {
	Graph graph;

	/* Initialize the graph. */
	graph_init(&graph, match_str, free);

	/* Perform some graph operations. */
	local_malloc_insert_vertex(&graph, "a");
	local_malloc_insert_vertex(&graph, "b");
	local_malloc_insert_vertex(&graph, "c");
	local_malloc_insert_vertex(&graph, "d");
	local_malloc_insert_vertex(&graph, "e");
	print_graph(&graph);

	local_malloc_insert_edge(&graph, "a", "b");
	local_malloc_insert_edge(&graph, "a", "c");
	local_malloc_insert_edge(&graph, "b", "c");
	local_malloc_insert_edge(&graph, "b", "d");
	local_malloc_insert_edge(&graph, "c", "b");
	local_malloc_insert_edge(&graph, "c", "c");
	local_malloc_insert_edge(&graph, "c", "d");
	local_malloc_insert_edge(&graph, "d", "a");
	local_malloc_insert_edge(&graph, "e", "c");
	local_malloc_insert_edge(&graph, "e", "d");
	print_graph(&graph);

	local_malloc_insert_edge(&graph, "a", "c");

	local_remove_edge(&graph, "a", "c");
	local_remove_edge(&graph, "c", "c");
	local_remove_edge(&graph, "e", "c");
	local_remove_edge(&graph, "a", "b");
	local_remove_edge(&graph, "d", "a");

	/* Testing the errors. */
	local_remove_vertex(&graph, "a");
	local_malloc_insert_edge(&graph, "f", "a");
	local_malloc_insert_edge(&graph, "c", "b");
	local_remove_edge(&graph, "f", "a");
	local_remove_edge(&graph, "c", "e");
	local_malloc_insert_vertex(&graph, "c");
	print_graph(&graph);

	/* Testing adjacent */
	local_check_adjacent(&graph, "b", "d");
	local_check_adjacent(&graph, "a", "e");
	local_check_adjacent(&graph, "e", "d");
	local_check_adjacent(&graph, "c", "a");

	/* Testing graph_adjlist of one vertex */
	local_check_adjlist(&graph, "c");
	local_check_adjlist(&graph, "a");

	/* Destroy the graph. */
	fprintf(stdout, "Destroying the graph\n");
	graph_destroy(&graph);

	return 0;
}
