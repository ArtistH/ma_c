/* lint.c */
#include "geometry.h"

/* lint */
int lint(Point p1, Point p2, Point p3, Point p4) {
	double z1;
	double z2;
	double z3;
	double z4;
	int s1;
	int s2;
	int s3;
	int s4;

	/* Perform the quick rejection test. */
	if (!(MAX(p1.x, p2.x) >= MIN(p3.x, p4.x)
		  && MAX(p3.x, p4.x) >= MIN(p1.x, p2.x)
		  && MAX(p1.y, p2.y) >= MIN(p3.y, p4.y)
		  && MAX(p3.y, p4.y) >= MIN(p1.y, p2.y))) {
		return 0;
	}

	/* Determine whether the line segments straddle each other. */
	z1 = ((p3.x - p1.x)*(p2.y - p1.y)) - ((p3.y - p1.y)*(p2.x - p1.x));
	if (z1 < 0) {
		s1 = -1;
	} else if (z1 > 0) {
		s1 = 1;
	} else {
		s1 = 0;
	}

	z2 = ((p4.x - p1.x)*(p2.y - p1.y)) - ((p4.y - p1.y)*(p2.x - p1.x));
	if (z2 < 0) {
		s2 = -1;
	} else if (z2 > 0) {
		s2 = 1;
	} else {
		s2 = 0;
	}

	z3 = ((p1.x - p3.x)*(p4.y - p3.y)) - ((p1.y - p3.y)*(p4.x - p3.x));
	if (z3 < 0) {
		s3 = -1;
	} else if (z3 > 0) {
		s3 = 1;
	} else {
		s3 = 0;
	}

	z4 = ((p2.x - p3.x)*(p4.y - p3.y)) - ((p2.y - p3.y)*(p4.x - p3.x));
	if (z4 < 0) {
		s4 = -1;
	} else if (z4 > 0) {
		s4 = 1;
	} else {
		s4 = 0;
	}

	if ((s1 * s2 <= 0) && (s3 * s4 <= 0)) {
		return 1;
	}

	/* Return that the line segments do not intersect. */
	return 0;
}
