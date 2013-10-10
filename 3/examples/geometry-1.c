/* ex-1.c for geometric algorithms. */
#include <math.h>
#include <stdio.h>
#include "Cbasic/geodist.h"
#include "Cbasic/geometry.h"
#include "Cbasic/list.h"

static void local_lint_printf(Point p1_rct, Point p2_rct,
							  Point p3_rct, Point p4_rct) {
	if (lint(p1_rct, p2_rct, p3_rct, p4_rct)) {
		fprintf(stdout, "(%+.1lf,%+.1lf) to (%+.1lf,%+.1lf) "
				"and (%+.1lf,%+.1lf) to (%+.1lf,%+.1lf) Y",
				p1_rct.x, p1_rct.y, p2_rct.x, p2_rct.y,
				p3_rct.x, p3_rct.y, p4_rct.x, p4_rct.y);
	} else {
		fprintf(stdout, "(%+.1lf,%+.1lf) to (%+.1lf,%+.1lf) "
				"and (%+.1lf,%+.1lf) to (%+.1lf,%+.1lf) N",
				p1_rct.x, p1_rct.y, p2_rct.x, p2_rct.y,
				p3_rct.x, p3_rct.y, p4_rct.x, p4_rct.y);
	}

	fprintf(stdout, " (N=OK)\n");
	return;
}

/* Define the number of examples for lint. */
#define NUM_INDEX 11
#define RCT_INDEX 8

/* Date for line examples. */
static double lint_data[NUM_INDEX][RCT_INDEX] = {
	{+2.0,-1.0,+4.0,-1.0,+1.0,-2.0,+3.0,+2.0},
	{-4.0,-3.0,-2.0,-1.0,-1.0,+0.0,+1.0,+2.0},
	{-4.0,-3.0,-2.0,-1.0,-4.0,-1.0,-3.0,-2.0},
	{-4.0,-3.0,-2.0,-1.0,-3.0,-2.0,-3.0,-2.0},
	{-3.0,-2.0,-3.0,-2.0,-3.0,-2.0,-3.0,-2.0},
	{-3.0,-2.0,+3.0,+4.0,-1.0,+2.0,+3.0,-1.0},
	{-3.0,-2.0,+3.0,+4.0,-1.0,-2.0,+3.0,+1.0},
	{-3.0,-2.0,+3.0,+4.0,-3.0,-2.0,-4.0,+3.0},
	{-3.0,-1.0,+3.0,+4.0,-3.0,-2.0,-4.0,+3.0},
	{-3.0,-2.0,+3.0,+4.0,-3.0,-1.0,-4.0,+3.0},
	{-3.0,+0.0,-4.0,+3.0,-5.0,+6.0,-6.0,+9.0}
};

static void test_lint() {
	Point p1_rct;
	Point p2_rct;
	Point p3_rct;
	Point p4_rct;

	/* Determine whether two line segments intersect. */
	fprintf(stdout, "Determining whether two line segments intersect\n");

	p1_rct.z = 0.0;
	p2_rct.z = 0.0;
	p3_rct.z = 0.0;
	p4_rct.z = 0.0;

	int i = 0;

	for (i = 0; i < NUM_INDEX; i++) {
		p1_rct.x = lint_data[i][0];
		p1_rct.y = lint_data[i][1];

		p2_rct.x = lint_data[i][2];
		p2_rct.y = lint_data[i][3];

		p3_rct.x = lint_data[i][4];
		p3_rct.y = lint_data[i][5];

		p4_rct.x = lint_data[i][6];
		p4_rct.y = lint_data[i][7];

		local_lint_printf(p1_rct, p2_rct, p3_rct, p4_rct);
	}

	fprintf(stdout, "\n");
	return;
}


/* Define data for computing convex hulls. */
#define CVXPCT 10

static Point CvxTestP[CVXPCT] = {
   { 0.0,  1.0, 0.0},
   {-3.0,  5.0, 0.0},
   {-2.0, -3.0, 0.0},
   { 0.0,  0.0, 0.0},
   { 3.0,  2.0, 0.0},
   {-5.0, -2.0, 0.0},
   { 5.0,  3.0, 0.0},
   {-3.0,  3.0, 0.0},
   { 2.0,  3.0, 0.0},
   {-3.0, -1.0, 0.0}
};

/* Compute a convex hull. */
static void test_cvxhull() {
	List P;
	List polygon;
	ListElmt *element = NULL;
	Point *point = NULL;
	int i = 0;

	fprintf(stdout, "Computing a convex hull\n");
	fprintf(stdout, "Points in P\n");

	list_init(&P, NULL);

	for (i = 0; i < CVXPCT; i++) {
		if (list_ins_next(&P, list_tail(&P), &CvxTestP[i]) != 0) {
			list_destroy(&P);
			return;
		}
		fprintf(stdout, "P[%03d]=(%+.1lf,%+.1lf,%+.1lf)\n",
				i, CvxTestP[i].x, CvxTestP[i].y, CvxTestP[i].z);
	}

	if (cvxhull(&P, &polygon) != 0) {
		list_destroy(&P);
		return;
	}

	fprintf(stdout, "Points in the convex hull\n");

	for (i = 0, element = list_head(&polygon);
		 element != NULL;
		 i++, element = list_next(element)) {
		point = list_data(element);
		fprintf(stdout, "polygon[%03d]=(%+.1lf,%+.1lf,%+.1lf)\n",
				i, point->x, point->y, point->z);
	}

	list_destroy(&P);
	list_destroy(&polygon);

	fprintf(stdout, "\n");
	return;
}

#define PLA_INDEX 12

/* Date for testing arclen */
static double PLACES[PLA_INDEX][3] = {
	{DEGTORAD(90.0), DEGTORAD(0.0), EARTH_RADIUS},
	{DEGTORAD(90.0), DEGTORAD(60.0), EARTH_RADIUS},
	{3606, 0, 0},

	/* SFO (San Francisco) */
	{DEGTORAD(90 - 37.62), DEGTORAD(-122.38), EARTH_RADIUS},
	/* LAX (Los Angeles) */
	{DEGTORAD(90.0 - 33.94), DEGTORAD(-118.41), EARTH_RADIUS},
	{293, 0, 0},

	/* SFO (San Francisco) */
	{DEGTORAD(90 - 37.62), DEGTORAD(-122.38), EARTH_RADIUS},
	/* HKG (Hong Kong) */
	{DEGTORAD(90.0 - 22.31), DEGTORAD(113.92), EARTH_RADIUS},
	{6159, 0, 0},

	/* CDG (Paris) */
	{DEGTORAD(90 - 49.01), DEGTORAD(2.55), EARTH_RADIUS},
	/* PER (Perth) */
	{DEGTORAD(90.0 + 31.94), DEGTORAD(115.97), EARTH_RADIUS},
	{7733, 0, 0}
};

/* Compute arc lengths on spherical surfaces. */
static void test_sph_arclen() {
	SPoint p1_sph;
	SPoint p2_sph;
	double length;
	int actual;

	fprintf(stdout, "Computing arc lengths on spherical surfaces\n");

	int i = 0;
	for (i = 0; i < PLA_INDEX; i += 3) {
		p1_sph.phi = PLACES[i][0];
		p1_sph.theta = PLACES[i][1];
		p1_sph.rho = PLACES[i][2];

		p2_sph.phi = PLACES[i+1][0];
		p2_sph.theta = PLACES[i+1][1];
		p2_sph.rho = PLACES[i+1][2];

		arclen (p1_sph, p2_sph, &length);
		actual = PLACES[i+2][0];

		fprintf(stdout, "Simple: phi=%+07.2lf, theta=%+07.2lf, rho=%.3lf\n",
				RADTODEG(p1_sph.phi), RADTODEG(p1_sph.theta), p1_sph.rho);

		fprintf(stdout, "Simple: phi=%+07.2lf, theta=%+07.2lf, rho=%.3lf\n",
				RADTODEG(p2_sph.phi), RADTODEG(p2_sph.theta), p2_sph.rho);

		fprintf(stdout, "length=%d, actual=%d, error=%6.4lf\n", (int)length,
				actual, fabs(1.0 - (floor(length) / actual)));

		fprintf(stdout, "\n");
	}

	return;
}

int main() {
	test_lint();
	test_cvxhull();
	test_sph_arclen();

	return 0;
}
