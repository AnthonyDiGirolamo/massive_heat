#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "grid.h"
#include "euler.h"
#include "solver.h"

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
