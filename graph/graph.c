/* graph.h */
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "../list.h"
#include "../set.h"

/* graph_init */
void graph_init(Graph *graph, int (*match)(const void *key1, 
	const void *key2), void (*destroy)(void *data))
{
	/* Initialize the graph. */
	graph->vcount = 0;
	graph->ecount = 0;
	graph->match = match;
	graph->destroy = destroy;

	/* Initialize the list of adjacency-list structures. */
	list_init(&graph->adjlists, NULL);

	return;
}
