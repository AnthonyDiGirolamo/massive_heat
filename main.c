#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grid.h"
#include "euler.h"

void set_initial_conditions(Grid grid) {
	for(int xi = 0; xi < grid.numstepsX; xi++) {
		U(xi,0) = sin(3.1415 * grid.hx * xi);
	}
}

void set_x_boundary_conditions(Grid grid, int x_index, double fixed_value) {
	for(int ti = 0; ti < grid.numstepsT; ti++) {
		U(x_index,ti) = fixed_value;
	}
}

Grid create_grid(int numstepsX, int numstepsT, double finalT) {
	Grid grid;
	grid.u = (double*)malloc(numstepsX * numstepsT * sizeof(double));
	grid.hx = 1.0/(numstepsX - 1);
	grid.ht = finalT/(numstepsT - 1);
	grid.numstepsX = numstepsX;
	grid.numstepsT = numstepsT;
	return grid;
}

void solve_heat_equation(Grid grid) {
	for (int ti = 1; ti < grid.numstepsT; ti++) {
		for (int xi = 1; xi < grid.numstepsX - 1; xi++) {
			U(xi,ti) = euler(xi, ti-1, grid);
		}
	}
}

void print_grid(Grid grid) {
	for (int ti = 0; ti < grid.numstepsT; ti++) {
		for (int xi = 0; xi < grid.numstepsX - 1; xi++) {
			printf("%f,",U(xi,ti));
		}
		printf("%f\n",U(grid.numstepsX - 1, ti));
	}
}

int main(int argc, const char** argv) {
	// Create grid
	Grid grid = create_grid(10,20,2.0);
	set_initial_conditions(grid);
	set_x_boundary_conditions(grid, 0, 0.0);
	set_x_boundary_conditions(grid, grid.numstepsX - 1, 0.0);

	// Do work
	solve_heat_equation(grid);
	print_grid(grid);
	return 0;
}
