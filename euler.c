#include "grid.h"
#include "euler.h"
#include <stdio.h>

/* Calculates u[x_index][t_index + 1] */
double euler(int x_index, int t_index, Grid grid) {
	double f = -0.01 * central_difference(x_index, t_index, grid);
	double y = U(x_index,t_index);
	return y + grid.ht*f;
}

double central_difference(int x_index, int t_index, Grid grid) {
	double left = U(x_index - 1,t_index);
	double central = U(x_index,t_index);
	double right = U(x_index + 1,t_index);
	return (right - 2.0*central + left)/(grid.hx * grid.hx);
}

