/* ex-2.c for breadth-first search */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Cbasic/bfs.h"
#include "Cbasic/graph.h"
#include "Cbasic/list.h"
#include "Cbasic/set.h"

/* Define the size of strings. */
#define STRSIZ 8

/* print_graph */
static void print_graph(const Graph *graph) {
	Set *adjacent = NULL;
	BfsVertex *vertex = NULL;
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

	return;
}

/* destroy_netid */
static void destroy_netid(void *netid) {
	/* Free the storage associated with the network id. */
	free(((BfsVertex *)netid)->data);
	free(netid);

	return;
}

/* match_netid */
static int match_netid(const void *netid1, const void *netid2) {
	/* Determine whether two network IDs match. */
	return !strcmp(((const BfsVertex *)netid1)->data,
					 ((const BfsVertex *)netid2)->data);
}

/* local_malloc_insert_network_vertex */
static void local_malloc_insert_network_vertex(Graph *network, char *string) {
	fprintf(stdout, "Inserting vertex %s\t\t\t", string);

	BfsVertex *netid = NULL;
	if ((netid = (BfsVertex *)malloc(sizeof(BfsVertex))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		return;
	}
	if ((netid->data = malloc(STRSIZ)) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(netid);
		return;
	}

	if (strlen(string) < STRSIZ) {
		strcpy((char *)netid->data, string);
	}

	int retval = graph_ins_vertex(network, netid);
	if (retval == 0) {
		fprintf(stdout, "\n");
		return;
	} else if (retval == -1) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(netid->data);
		free(netid);
		return;
	} else if (retval == 1) {
		fprintf(stdout, "--- [ERROR] : vertex exist\n");
		free(netid->data);
		free(netid);
		return;
	}

	return;
}

/* local_malloc_insert_network_edge */
static void local_malloc_insert_network_edge(Graph *network,
											 char *string1, char *string2) {
	fprintf(stdout, "Inserting edge %s --> %s\t\t", string1, string2);

	BfsVertex *netid1 = NULL;
	if ((netid1 = (BfsVertex *)malloc(sizeof(BfsVertex))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		return;
	}
	char data1[STRSIZ];
	if (strlen(string1) < STRSIZ) {
		strcpy(data1, string1);
	}
	netid1->data = data1;

	BfsVertex *netid2 = NULL;
	if ((netid2 = (BfsVertex *)malloc(sizeof(BfsVertex))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(netid1);
		return;
	}
	if ((netid2->data = malloc(STRSIZ)) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		free(netid1);
		free(netid2);
		return;
	}
	if (strlen(string2) < STRSIZ) {
		strcpy(netid2->data, string2);
	}

	int retval = graph_ins_edge(network, netid1, netid2);
	if (retval == 0) {
		fprintf(stdout, "\n");
		free(netid1);
		return;
	} else if (retval == -1) {
		fprintf(stdout, "--- [ERROR] : vertex not exist\n");
		free(netid1);
		free(netid2->data);
		free(netid2);
		return;
	} else if (retval == 1) {
		fprintf(stdout, "--- [ERROR] : edge already exist\n");
		free(netid1);
		free(netid2->data);
		free(netid2);
		return;
	}

	return;
}

/* local_bfs_output */
static void local_bfs_output(Graph *network, char *str_start, List *list) {
	fprintf(stdout, "Generating the breadth-first search listing\t\t");
	BfsVertex *netid = NULL;
	if ((netid = (BfsVertex *)malloc(sizeof(BfsVertex))) == NULL) {
		fprintf(stdout, "--- [ERROR] : malloc error\n");
		return;
	}
	char data1[STRSIZ];
	if (strlen(str_start) < STRSIZ) {
		strcpy(data1, str_start);
	}
	netid->data = data1;

	int retval = bfs(network, netid, list);
	if (retval == 0) {
		fprintf(stdout, "\n");
		free(netid);
	} else {
		fprintf(stdout, "--- [ERROR] : error happens inside\n");
		free(netid);
		return;
	}

	/* Output the bfs-output. */
	/* netid reuse only the parameter but not the space. */
	int i = 0;
	ListElmt *element = NULL;
	for (i = 0, element = list_head(list);
		 element != NULL;
		 i++, element = list_next(element)) {
		netid = list_data(element);
		fprintf(stdout, "\tList[%03d]=%s (color=%d, hops=%d)\n", i,
				(char *)netid->data, netid->color, netid->hops);
	}

	return;
}

/* main */
int main() {
	Graph network;
	List list;

	/* Initialize the graph. */
	graph_init(&network, match_netid, destroy_netid);

	/* Insert some network IDs. */
	local_malloc_insert_network_vertex(&network, "node1");
	local_malloc_insert_network_vertex(&network, "node2");
	local_malloc_insert_network_vertex(&network, "node3");
	local_malloc_insert_network_vertex(&network, "node4");
	local_malloc_insert_network_vertex(&network, "node5");
	local_malloc_insert_network_vertex(&network, "node6");
	local_malloc_insert_network_vertex(&network, "node6");
	print_graph(&network);

	/* Insert some network connections. */
	local_malloc_insert_network_edge(&network, "node1", "node2");
	local_malloc_insert_network_edge(&network, "node1", "node2");
	local_malloc_insert_network_edge(&network, "node2", "node1");
	local_malloc_insert_network_edge(&network, "node1", "node3");
	local_malloc_insert_network_edge(&network, "node3", "node1");
	local_malloc_insert_network_edge(&network, "node2", "node3");
	local_malloc_insert_network_edge(&network, "node3", "node2");
	local_malloc_insert_network_edge(&network, "node2", "node4");
	local_malloc_insert_network_edge(&network, "node4", "node2");
	local_malloc_insert_network_edge(&network, "node3", "node5");
	local_malloc_insert_network_edge(&network, "node5", "node3");
	local_malloc_insert_network_edge(&network, "node4", "node5");
	local_malloc_insert_network_edge(&network, "node5", "node4");
	local_malloc_insert_network_edge(&network, "node7", "node4");
	local_malloc_insert_network_edge(&network, "node5", "node6");
	local_malloc_insert_network_edge(&network, "node6", "node5");
	print_graph(&network);

	/* Perform breadth-first search. */
	local_bfs_output(&network, "node1", &list);

	/* Destroy the linked list. */
	fprintf(stdout, "Destroying the list\n");
	list_destroy(&list);

	/* Destroy the graph. */
	fprintf(stdout, "Destroying the graph\n");
	graph_destroy(&network);

	return 0;
}
