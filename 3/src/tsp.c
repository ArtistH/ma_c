/* tsp.c */
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "Cbasic/graph.h"
#include "Cbasic/graphalg.h"
#include "Cbasic/list.h"

/* tsp */
int tsp(List *vertices, const TspVertex *start, List *tour,
		int (*match)(const void *key1, const void *key2)) {
	TspVertex *tsp_vertex = NULL;
	TspVertex *tsp_start = NULL;
	TspVertex *selection = NULL;
	ListElmt *element = NULL;
	double minimum;
	double distance;
	double x = 0.0;
	double y = 0.0;
	int found;
	int i;

	/* Initialize the list for the tour. */
	list_init(tour, NULL);

	/* Initialize all of the vertices in the graph. */
	found = 0;

	for (element = list_head(vertices);
		 element != NULL;
		 element = list_next(element)) {
		tsp_vertex = list_data(element);
		if (match(tsp_vertex, start)) {
			/* Start the tour at the start vertex. */
			if (list_ins_next(tour, list_tail(tour), tsp_vertex) != 0) {
				list_destroy(tour);
				return -1;
			}
			/* Save the start vertex and its coordinates. */
			tsp_start = tsp_vertex;
			x = tsp_vertex->x;
			y = tsp_vertex->y;
			/* Color the static vertex black. */
			tsp_vertex->color = black;
			found = 1;
		} else {
			/* Color all other vertices white. */
			tsp_vertex->color = white;
		}
	}

	/* Return if the start vertex was not found. */
	if (!found) {
		list_destroy(tour);
		return -1;
	}

	/* Use the nearest-neighbor heuristic to compute the tour. */
	for (i = 0; i < list_size(vertices) - 1; i++) {
		/* Select the white vertex closest to the previous vertex
		 * in the tour. */
		minimum = DBL_MAX;
		for (element = list_head(vertices);
			 element != NULL;
			 element = list_next(element)) {
			tsp_vertex = list_data(element);
			if (tsp_vertex->color == white) {
				distance = sqrt(pow(tsp_vertex->x - x, 2.0)
								+ pow(tsp_vertex->y - y, 2.0));
				if (distance < minimum) {
					minimum = distance;
					selection = tsp_vertex;
				}
			}
		}

		/* Save the coordinates of the selected vertex. */
		x = selection->x;
		y = selection->y;

		/* Color the selected vertex black. */
		selection->color = black;

		/* Insert the selected vertex into the tour. */
		if (list_ins_next(tour, list_tail(tour), selection) != 0) {
			list_destroy(tour);
			return -1;
		}
	}

	/* Insert the start vertex again to complete the tour. */
	if (list_ins_next(tour, list_tail(tour), tsp_start) != 0) {
		list_destroy(tour);
		return -1;
	}

	return 0;
}
