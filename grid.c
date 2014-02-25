void print_grid(Grid grid) {
	for (int ti = 0; ti < grid.numstepsT; ti++) {
		for (int xi = 0; xi < grid.numstepsX - 1; xi++) {
			printf("%f,",U(xi,ti));
		}
		printf("%f\n",U(grid.numstepsX - 1, ti));
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
