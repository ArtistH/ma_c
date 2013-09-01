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
#define PTHVCT 6
#define PTHECT 11

static char PthTestV[PTHVCT][STRSIZ] = {
	"a", "b", "c", "d", "e", "f"
};

static EdgeData PthTestE[PTHECT] = {
	{"a", "b", 8.0},
	{"a", "c", 4.0},
	{"b", "c", 6.0},
	{"b", "d", 2.0},
	{"b", "f", 4.0},
	{"c", "e", 4.0},
	{"c", "f", 1.0},
	{"e", "f", 5.0},
	{"f", "b", 2.0},
	{"f", "d", 7.0},
	{"f", "e", 4.0},
};

/* Define data for traveling-salesman problems. */

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

/* print_graph_pth */
static void print_graph_pth(const Graph *graph) {
	Set *adjacent = NULL;
	PathVertex *vertex = NULL;
	ListElmt *element = NULL;
	ListElmt *member = NULL;
	int i;
	int j;

	/* Display the graph for computing shortest paths. */
	fprintf(stdout, "==> Vertices=%d, edges=%d\n",
			graph_vcount(graph), graph_ecount(graph));

	for (i = 0, element = list_head(&graph_adjlists(graph));
		 i < list_size(&graph_adjlists(graph));
		 i++, element = list_next(element)) {
		vertex = ((AdjList *)list_data(element))->vertex;
		fprintf(stdout, "\t graph[%03d]=%s: ", i, (char *)vertex->data);

		adjacent = &((AdjList *)list_data(element))->adjacent;
		for (j = 0, member = list_head(adjacent);
			 j < set_size(adjacent);
			 j++, member = list_next(member)) {
			vertex = list_data(member);
			if (j > 0) {
				fprintf(stdout, ", ");
			}
			fprintf(stdout, "%s(%4.1lf)",
					(char *)vertex->data, vertex->weight);
		}

		fprintf(stdout, "\n");
	}

	return;
}

/* match_pth */
static int match_pth(const void *pth1, const void *pth2) {
	/* Determine whether the data in two PathVertex vertices match. */
	return !strcmp(((const PathVertex *)pth1)->data,
				   ((const PathVertex *)pth2)->data);
}

/* main */
int main() {
	Graph graph;

	MstVertex *mst_start = NULL;
	MstVertex *mst_vertex = NULL;
	MstVertex mst_vertex1;
	MstVertex *mst_vertex2 = NULL;

	PathVertex *pth_start = NULL;
	PathVertex *pth_vertex = NULL;
	PathVertex pth_vertex1;
	PathVertex *pth_vertex2 = NULL;

	List span;
	List paths;

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

	/* Compute shortest paths. */
	graph_init(&graph, match_pth, free);
	fprintf(stdout, "Computing shortest paths\n");

	for (i = 0; i < PTHVCT; i++) {
		if ((pth_vertex = (PathVertex *)malloc(sizeof(PathVertex))) == NULL) {
			return 1;
		}
		if (i == 1) {
			pth_start = pth_vertex;
		}
		pth_vertex->data = PthTestV[i];
		if (graph_ins_vertex(&graph, pth_vertex) != 0) {
			return 1;
		}
	}

	for (i = 0; i < PTHECT; i++) {
		if ((pth_vertex2 = (PathVertex *)malloc(sizeof(PathVertex))) == NULL) {
			return 1;
		}
		pth_vertex1.data = PthTestE[i].vertex1;
		pth_vertex2->data = PthTestE[i].vertex2;
		pth_vertex2->weight = PthTestE[i].weight;
		if (graph_ins_edge(&graph, &pth_vertex1, pth_vertex2) != 0) {
			return 1;
		}
	}

	print_graph_pth(&graph);

	if (shortest(&graph, pth_start, &paths, match_pth) != 0) {
		return 1;
	}

	for (element = list_head(&paths);
		 element != NULL;
		 element = list_next(element)) {
		pth_vertex = list_data(element);
		fprintf(stdout, "vertex=%s, parent=%s, d=%.1lf\n",
				(char *)pth_vertex->data,
				pth_vertex->parent != NULL ? (char *)pth_vertex->parent->data : "*",
				pth_vertex->d);
	}

	list_destroy(&paths);
	graph_destroy(&graph);

	return 0;
}
