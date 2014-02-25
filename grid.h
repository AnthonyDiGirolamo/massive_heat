#ifndef GRID
#define GRID 1
#define U(XI,TI) grid.u[XI + TI*grid.numstepsX]
typedef struct Grid {
	double* u;
	double hx;
	double ht;
	int numstepsX;
	int numstepsT;
} Grid;
void print_grid(Grid grid);
Grid create_grid(int numstepsX, int numstepsT, double finalT);
#endif
