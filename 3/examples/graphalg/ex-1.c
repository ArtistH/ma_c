/* ex-1.c for graph-algorithms(mst, shortest, tsp) */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "graphalg.h"
#include "list.h"
#include "set.h"

#define STRSIZ 2

/* Define a structure for edge data in graphs. */
typedef struct EdgeData_ {
	char vertex1[STRSIZ];
	char vertex2[STRSIZ];
	double weight;
} EdgeData;

/* Define a structure for coordinate data in graphs. */
typedef struct CoordData_ {
	char vertex[STRSIZ];
	double x;
	double y;
} CoordData;

/* Define data for computing minimum spanning trees. */
#define MSTVCT 9
#define MSTECT 28

static char MstTestV[MSTVCT][MSTVCT] = {
	"a", "b", "c", "d", "e", "f", "g", "h", "i"
};

static EdgeData MstTestE[MSTECT] = {
	{"a", "b", 4.0},
	{"a", "h", 8.0},
	{"b", "a", 4.0},
	{"b", "c", 8.0},
	{"b", "h", 11.0},
	{"c", "b", 8.0},
	{"c", "d", 7.0},
	{"c", "f", 4.0},
	{"c", "i", 2.0},
	{"d", "c", 7.0},
	{"d", "e", 9.0},
	{"d", "f", 14.0},
	{"e", "d", 9.0},
	{"e", "f", 10.0},
	{"f", "c", 4.0},
	{"f", "d", 14.0},
	{"f", "e", 10.0},
	{"f", "g", 2.0},
	{"g", "f", 2.0},
	{"g", "h", 1.0},
	{"g", "i", 6.0},
	{"h", "a", 8.0},
	{"h", "b", 11.0},
	{"h", "g", 1.0},
	{"h", "i", 7.0},
	{"i", "c", 2.0},
	{"i", "g", 6.0},
	{"i", "h", 7.0}
};

/* Define data for computing shortest paths. */

/* print_graph_mst */
static void print_graph_mst(const Graph *graph) {
	Set *adjacent = NULL;
	MstVertex *vertex = NULL;
	ListElmt *element = NULL;
	ListElmt *member = NULL;
	int i;
	int j;

	/* Display the graph for a minimum spanning tree. */
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
			fprintf(stdout, "%s(%4.1lf)", (char *)vertex->data,
					vertex->weight);
		}

		fprintf(stdout, "\n");
	}
	return;
}

/* match_mst */
static int match_mst(const void *mst1, const void *mst2) {
	/* Determine whether the data in two MstVertex vertices match. */
	return !strcmp(((const MstVertex *)mst1)->data,
				   ((const MstVertex *)mst2)->data);
}

/* main */
int main() {
	Graph graph;

	MstVertex *mst_start = NULL;
	MstVertex *mst_vertex = NULL;
	MstVertex mst_vertex1;
	MstVertex *mst_vertex2 = NULL;

	List span;

	ListElmt *element = NULL;

	int i;

	/* Compute a minimum spanning tree. */
	graph_init(&graph, match_mst, free);
	fprintf(stdout, "Computing a minimum spanning tree\n");

	for (i = 0; i < MSTVCT; i++) {
		if ((mst_vertex = (MstVertex *)malloc(sizeof(MstVertex))) == NULL) {
			return 1;
		}
		if (i == 0) {
			mst_start = mst_vertex;
		}
		mst_vertex->data = MstTestV[i];
		if (graph_ins_vertex(&graph, mst_vertex) != 0) {
			return 1;
		}
	}

	for (i = 0; i < MSTECT; i++) {
		if ((mst_vertex2 = (MstVertex *)malloc(sizeof(MstVertex))) == NULL) {
			return 1;
		}
		mst_vertex1.data = MstTestE[i].vertex1;
		mst_vertex2->data = MstTestE[i].vertex2;
		mst_vertex2->weight = MstTestE[i].weight;
		if (graph_ins_edge(&graph, &mst_vertex1, mst_vertex2) != 0) {
			return 1;
		}
	}

	print_graph_mst(&graph);

	if (mst(&graph, mst_start, &span, match_mst) != 0) {
		return 1;
	}
	for (element = list_head(&span);
		 element != NULL;
		 element = list_next(element)) {
		mst_vertex = list_data(element);
		fprintf(stdout, "\tvertex=%s, parent=%s\n", (char *)mst_vertex->data,
				mst_vertex->parent != NULL ? (char *)mst_vertex->parent->data : "*");
	}

	list_destroy(&span);
	graph_destroy(&graph);

	return 0;
}
