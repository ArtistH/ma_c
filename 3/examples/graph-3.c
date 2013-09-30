/* ex-3.c for depth-first search dfs.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfs.h"
#include "graph.h"
#include "list.h"
#include "set.h"

/* Define the size of strings. */
#define STRSIZ 8

/* print_graph */
static void print_graph(const Graph *graph) {
	Set *adjacent = NULL;
	DfsVertex *vertex = NULL;
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
		vertex = ((AdjList *)list_data(element))->vertex;
		fprintf(stdout, "\tgraph[%03d]=%s: ", i, (char *)vertex->data);
		adjacent = &((AdjList *)list_data(element))->adjacent;
		for (j = 0, member = list_head(adjacent);
			 j < set_size(adjacent);
			 j++, member = list_next(member)) {
			vertex = list_data(member);
			if (j > 0) {
				fprintf(stdout, ", ");
			}
			fprintf(stdout, "%s", (char *)vertex->data);
		}
		fprintf(stdout, "\n");
	}
}

/* destroy_task */
static void destroy_task(void *task) {
	/* Free the storage associated with the task. */
	free(((DfsVertex *)task)->data);
	free(task);
}

/* match_task */
static int match_task(const void *task1, const void *task2) {
	/* Determine whether two tasks match. */
	return !strcmp(((const DfsVertex *)task1)->data,
				   ((const DfsVertex *)task2)->data);
}

/* local_malloc_insert_vertex */
static void local_malloc_insert_vertex(Graph *graph, char *string) {
	fprintf(stdout, "Inserting vertex %s\t\t", string);
	DfsVertex *task = NULL;
	if ((task = (DfsVertex *)malloc(sizeof(DfsVertex))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		return;
	}

	if ((task->data = (char *)malloc(sizeof(STRSIZ))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(task);
		return;
	}
	if (strlen(string) < STRSIZ) {
		strcpy(task->data, string);
	}
	
	int retval = graph_ins_vertex(graph, task);
	if (retval == 0) {
		fprintf(stdout, "\n");
		return;
	} else if (retval == -1) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(task->data);
		free(task);
		return;
	} else if (retval == 1) {
		fprintf(stdout, "--- [ERROR] : vertex exist\n");
		free(task->data);
		free(task);
		return;
	}

	return;
}

/* local_malloc_insert_edge */
static void local_malloc_insert_edge(Graph *network,
									 char *string1, char *string2) {
	fprintf(stdout, "Inserting edge %s --> %s\t\t", string1, string2);

	DfsVertex *task1 = NULL;
	if ((task1 = (DfsVertex *)malloc(sizeof(DfsVertex))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		return;
	}
	char data1[STRSIZ];
	if (strlen(string1) < STRSIZ) {
		strcpy(data1, string1);
	}
	task1->data = data1;

	DfsVertex *task2 = NULL;
	if ((task2 = (DfsVertex *)malloc(sizeof(DfsVertex))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(task1);
		return;
	}
	if ((task2->data = malloc(STRSIZ)) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(task1);
		free(task2);
		return;
	}
	if (strlen(string2) < STRSIZ) {
		strcpy(task2->data, string2);
	}

	int retval = graph_ins_edge(network, task1, task2);
	if (retval == 0) {
		fprintf(stdout, "\n");
		free(task1);
		return;
	} else if (retval == -1) {
		fprintf(stdout, "--- [ERROR] : vertex not exist\n");
		free(task1);
		free(task2->data);
		free(task2);
		return;
	} else if (retval == 1) {
		fprintf(stdout, "--- [ERROR] : edge already exist\n");
		free(task1);
		free(task2->data);
		free(task2);
		return;
	}

	return;
}

/* local_dfs_output */
static void local_dfs_output(Graph *graph, List *ordered) {
	fprintf(stdout, "Generating the depth-first search listing\t\t");

	int retval = dfs(graph, ordered);
	if (retval == 0) {
		fprintf(stdout, "\n");
	} else {
		fprintf(stdout, "--- [ERROR] : error happens inside\n");
		return;
	}

	/* Output the bfs-output. */
	/* task reuse only the parameter but not the space. */
	int i = 0;
	DfsVertex *task = NULL;
	ListElmt *element = NULL;
	for (i = 0, element = list_head(ordered);
		 element != NULL;
		 i++, element = list_next(element)) {
		task = list_data(element);
		fprintf(stdout, "\tList[%03d]=%s (color=%d)\n", i,
				(char *)task->data, task->color);
	}

	return;
}

/* main */
int main() {
	Graph graph;
	List list;

	/* Initialize the graph. */
	graph_init(&graph, match_task, destroy_task);

	/* Insert some tasks. */
	local_malloc_insert_vertex(&graph, "a");
	local_malloc_insert_vertex(&graph, "b");
	local_malloc_insert_vertex(&graph, "c");
	local_malloc_insert_vertex(&graph, "d");
	local_malloc_insert_vertex(&graph, "e");
	local_malloc_insert_vertex(&graph, "f");
	local_malloc_insert_vertex(&graph, "g");
	local_malloc_insert_vertex(&graph, "h");
	local_malloc_insert_vertex(&graph, "i");
	print_graph(&graph);

	/* Insert some dependencies. */
	local_malloc_insert_edge(&graph, "a", "b");
	local_malloc_insert_edge(&graph, "a", "c");
	local_malloc_insert_edge(&graph, "b", "i");
	local_malloc_insert_edge(&graph, "c", "i");
	local_malloc_insert_edge(&graph, "e", "f");
	local_malloc_insert_edge(&graph, "e", "h");
	local_malloc_insert_edge(&graph, "f", "c");
	local_malloc_insert_edge(&graph, "f", "h");
	local_malloc_insert_edge(&graph, "g", "h");
	local_malloc_insert_edge(&graph, "a", "b");
	print_graph(&graph);

	/* Perform depth-first search. */
	local_dfs_output(&graph, &list);

	/* Destroy the linked list. */
	fprintf(stdout, "Destroying the list\n");
	list_destroy(&list);

	/* Destroy the graph. */
	fprintf(stdout, "Destroying the graph\n");
	graph_destroy(&graph);

	return 0;
}
