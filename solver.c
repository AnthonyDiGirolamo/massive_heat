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



void solve_heat_equation(Grid grid) {
	for (int ti = 1; ti < grid.numstepsT; ti++) {
		for (int xi = 1; xi < grid.numstepsX - 1; xi++) {
			U(xi,ti) = euler(xi, ti-1, grid);
		}
	}
}

