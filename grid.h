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
#endif
