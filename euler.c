#include "euler.h"

/* Calculates u[x_index][t_index + 1] */
double euler(int x_index, int t_index, double** u, double h) {
	double f = -1 * central_difference(x_index, t_index, u, h);
	double y = u[x_index][t_index];
	return y + h*f;
}

double central_difference(int x_index, int t_index, double** u, double h) {
	double left = u[x_index - 1][t_index];
	double central = u[x_index][t_index];
	double right = u[x_index + 1][t_index];
	return (left - 2*central + right)/(h * h);
}

