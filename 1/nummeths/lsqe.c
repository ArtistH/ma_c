/* lsqe.c */
/* O(n)
 * 采用最小二乘法计算y(x) = b1*x + b0中的b1,b0 */
#include <math.h>
#include "nummeths.h"

/* lsqe */
void lsqe(const double *x, const double *y, int n, double *b1, double *b0)
{
	double sumx, sumy, sumx2, sumxy;
	int i;

	/* Compute the required summations. */
	sumx = 0.0;
	sumy = 0.0;
	sumx2 = 0.0;
	sumxy = 0.0;

	for (i = 0; i < n; i++) {
		sumx = sumx + x[i];
		sumy = sumy + y[i];
		sumx2 = sumx2 + pow(x[i], 2.0);
		sumxy = sumxy + (x[i] * y[i]);
	}

	/* Compute the least-square estmators. */
	*b1 = (sumxy - ((sumx * sumy) / (double)n)) / (sumx2 - (pow(sumx, 2.0) / (double)n));
	*b0 = (sumy - ((*b1) * sumx)) / (double)n;

	return;
}