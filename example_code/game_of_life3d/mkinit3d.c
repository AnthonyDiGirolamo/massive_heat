// Anthony DiGirolamo
// Random Initial Condition Generator

#define INITIAL "initial_condition.txt"
#define LIVE '1'
#define DEAD '0'

//#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>

int main(int argc, char *argv[] ) {

	FILE *file;

	int r;	// number of rows
	int c;	// number of columns
	int p;	// number of columns

	int i = 0, row = 0, col = 0, pln = 0;	// array iterators
		  	
	if (argc != 4) {
		fputs("usage: <x> <y> <z>\n", stderr);
		exit(EXIT_FAILURE);
	}
	
	r = (int) strtol(argv[1], NULL, 10);
	c = (int) strtol(argv[2], NULL, 10);
	p = (int) strtol(argv[2], NULL, 10);
	
	file = fopen(INITIAL, "w");
	if (file == NULL) {
		fputs(INITIAL, stderr);
		fputs(" could not be opened.\n", stderr);
		exit(EXIT_FAILURE);		
	}
	
	srand(time(0));
	
	// Make a text image
	for (row=0; row<r; row++) {
		for (col=0; col<c; col++) {
			for (pln=0; pln<p; pln++) {
				if (rand()%2 == 0)
					fputc(DEAD, file);
				else
					fputc(LIVE, file);
			}
		}
	}
	
	fclose(file);		
	
	return EXIT_SUCCESS;
}
