// Anthony DiGirolamo
// Game Of Life 3D

#define INITIAL "initial_condition.txt"
#define CHECKPOINT "checkpoint.txt"
#define FINAL_RESULTS "final_results.txt"

#include <mpi.h>
// #include <mpe.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// SPRNG Setup

/*
#define SIMPLE_SPRNG
#define SPRNG_DEFAULT
#define USE_MPI
#include "sprng.h"
*/

int liveOrDie(int cell, int neighbors) {
	cell -= 48; neighbors -= 384; // convert values from chars to ints, (48 = 0, 49 = 1)
	int newCell = 0;
	
	if (cell == 0) {
		if (neighbors == 3)
			newCell = 1;
	}
	else {
		if (neighbors == 2 || neighbors == 3)
			newCell = 1;
		else if (neighbors < 2 || neighbors > 3)
			newCell = 0;
	}
	
	return newCell+48; // back into a character
}

void gameOfLife(char **array1, char **array2, int rows, int cols, int l, int rank) {				
	int r, c, i;
	double rnd;
	
	for (r = 1; r < rows+1; r++) {		
		for (c = 1; c < cols+1; c++) {			
						
			array2[r][c] = liveOrDie(array1[r][c], 
							array1[r-1][c-1] + array1[r-1][c] + array1[r-1][c+1] +
							array1[r][c-1]   +                  array1[r][c+1]   +
							array1[r+1][c+1] + array1[r+1][c] + array1[r+1][c+1] );								
						
			for(i=0; i<l; i++) {  // generate random numbers
				rnd = rand(); //sprng();
				
				if (rnd <= 0.001) {
					array2[r][c] = '1';  // random_life_created!
					if (rnd <= 0.0001)
						array2[r][c] = '0';	 // life_struck_by_lightning!
				}
			}
		}		
	}
}

int main(int argc, char *argv[] ) {
	double time1, time2;
	
	time1 = MPI_Wtime();

	int rank, processors;
	
	int j;	// number of iterations
	int k;	// number of iterations to perform before creating a checkpoint
	int l;  // number of random samples per grid point
	int checkpoint_resume = 0;	// 1 = resume from last checkpoint

	int c;		// used to hold a character
	int i=0, row = 0, col = 0, pln = 0;	// array iterators

	char ***local_array;		   
	char **local_array_2nd;		   
	char *local_array_pointer; 

	char ***local_array_copy;		   
	char **local_array_copy_2nd;		   
	char *local_array_copy_pointer; 

	char ***temp, *temp_pointer;
	
	int file_open_error;
	int command_line_incomplete = 0;

	int grid_size[3] 	  = {0,0,0};
	int proc_size[3] 	  = {0,0,0};
	int local_size[3] 	  = {0,0,0};
	int remainder_size[3] = {0,0,0};
	int coords[3] 		  = {0,0,0};
	int start_indices[3]  = {0,0,0};
	int periods[3]        = {0,0,0};
	int mem_size[3]       = {0,0,0};
	
	MPI_Status status;
	MPI_Datatype filetype, memtype;
	MPI_File fh;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &processors);	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	

	// Interpret the command line arguments --------------------------------
  	if (rank == 0) {  	  	
		
		if (argc < 6 || argc > 8) {
			fputs("usage: x y z j k l r\n", stderr);
			fputs("where: x,y,z = x, y and z dimensions\n", stderr);
			fputs("       j = how many times the game of life is played\n", stderr);
			fputs("       k = checkpoint every k iterations\n", stderr);
			fputs("       l = number of random samples per grid point\n", stderr);
			fputs("       r = resume from the last checkpoint\n", stderr);
			fputs(INITIAL, stderr);
			fputs(" must be present.\n", stderr);
			fputs(CHECKPOINT, stderr);
			fputs(" must be present if resuming from the last checkpoint.\n", stderr);
			exit(EXIT_FAILURE);
		}

  	}

	j = (int) strtol(argv[4], NULL, 10);
	k = (int) strtol(argv[5], NULL, 10);
	l = (int) strtol(argv[6], NULL, 10);		
	if ( argc == 7 )
		if ( argv[6][0] == 'r' )
			checkpoint_resume = 1;			

	if (rank == 0)
		printf("%d iterations \ncheckpoint every %d iterations \n%d samples per grid point \ncheckpoint resume = %d\n", j,k,l,checkpoint_resume);				
	
	grid_size[0] = (int) strtol(argv[1], NULL, 10);
	grid_size[1] = (int) strtol(argv[2], NULL, 10);
	grid_size[2] = (int) strtol(argv[3], NULL, 10);
	if (rank==0) printf("grid_size: %d, %d, %d\n", grid_size[0], grid_size[1], grid_size[2]);

	MPI_Dims_create(processors, 3, proc_size);
	if (rank==0) printf("proc_size: %d, %d, %d\n", proc_size[0], proc_size[1], proc_size[2]);

	local_size[0] = grid_size[0] / proc_size[0];
	local_size[1] = grid_size[1] / proc_size[1];
	local_size[2] = grid_size[2] / proc_size[2];
	if (rank==0) printf("local_size: %d, %d, %d\n", local_size[0], local_size[1], local_size[2]);

	remainder_size[0] = grid_size[0] % proc_size[0];
	remainder_size[1] = grid_size[1] % proc_size[1];
	remainder_size[2] = grid_size[2] % proc_size[2];
	if (rank==0) printf("remainder_size: %d, %d, %d\n", remainder_size[0], remainder_size[1], remainder_size[2]);
	if (remainder_size[0] != 0 || remainder_size[1] != 0 || remainder_size[2] != 0) {
		fputs("remainder size != 0, check your dimensions", stderr);
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}

	MPI_Comm comm;
	MPI_Cart_create(MPI_COMM_WORLD, 3, proc_size, periods, 0, &comm);
	MPI_Comm_rank(comm, &rank);
	MPI_Cart_coords(comm, rank, 3, coords);

	start_indices[0] = coords[0] * local_size[0];
	start_indices[1] = coords[1] * local_size[1];
	start_indices[2] = coords[2] * local_size[2];

/*	printf("A coords R%d: (%d, %d, %d)  (%d, %d, %d)\n", rank, coords[0], coords[1], coords[2], start_indices[0], start_indices[1], start_indices[2]);*/
	fflush(stdout);
	
	// create the file type ---------------------------------------------------
	MPI_Type_create_subarray(3, grid_size, local_size, start_indices, MPI_ORDER_C, MPI_CHAR, &filetype); 
	MPI_Type_commit(&filetype);
	
	// create a local memory type with ghost rows -----------------------------
	mem_size[0] = local_size[0] + 2; 
	mem_size[1] = local_size[1] + 2; 
	mem_size[2] = local_size[2] + 2; 
	start_indices[0] = start_indices[1] = start_indices[2] = 1;
	
	MPI_Type_create_subarray(3, mem_size, local_size, start_indices, MPI_ORDER_C, MPI_CHAR, &memtype);
	MPI_Type_commit(&memtype);
	
	// find my neighbors ------------------------------------------------------

	int nxminus, nxplus, nyminus, nyplus, nzminus, nzplus, tag = 333, *neighbors;

	// Neighbors Array:  row-  col-  col+  row+  plane-  plane+

	neighbors = (int *) malloc(6 * sizeof(int));
	for(i=0; i<6; i++)
		neighbors[i] = rank;

	MPI_Cart_shift(comm, 0, 1, &nxminus, &nxplus);
	MPI_Cart_shift(comm, 1, 1, &nyminus, &nyplus);
	MPI_Cart_shift(comm, 2, 1, &nzminus, &nzplus);

//	printf(" %d sending south to %d receiving from %d \n",rank,nxplus,nxminus);
//	fflush(stdout);
	MPI_Sendrecv(&rank, 1, MPI_INT, nxplus, tag, 
		&(neighbors[0]), 1, MPI_INT, nxminus, tag, comm, &status);

//	printf(" %d sending North to %d receiving from %d \n",rank,nxminus,nxplus);
//	fflush(stdout);
	MPI_Sendrecv(&rank, 1, MPI_INT, nxminus, tag, 
		&(neighbors[3]), 1, MPI_INT, nxplus, tag, comm, &status);

//	printf(" %d sending East to %d receiving from %d \n",rank,nyplus,nyminus);
//	fflush(stdout);
	MPI_Sendrecv(&rank, 1, MPI_INT, nyplus, tag, 
		&neighbors[1], 1, MPI_INT, nyminus, tag, comm, &status);

//	printf(" %d sending West to %d receiving from %d \n",rank,nyminus,nyplus);
//	fflush(stdout);
	MPI_Sendrecv(&rank, 1, MPI_INT, nyminus, tag, 
		&neighbors[2], 1, MPI_INT, nyplus, tag, comm, &status);

//	printf(" %d sending backwards to %d receiving from %d \n",rank,nzplus,nzminus);
//	fflush(stdout);
	MPI_Sendrecv(&rank, 1, MPI_INT, nzplus, tag, 
		&(neighbors[4]), 1, MPI_INT, nzminus, tag, comm, &status);

//	printf(" %d sending forward to %d receiving from %d \n",rank,nzminus,nzplus);
//	fflush(stdout);
	MPI_Sendrecv(&rank, 1, MPI_INT, nzminus, tag, 
		&(neighbors[5]), 1, MPI_INT, nzplus, tag, comm, &status);

/*	printf("neighboors R%d : (row-) %d (col-) %d (col+) %d (row+) %d (plane-) %d (plane+) %d\n",rank,neighbors[0],neighbors[1],neighbors[2],neighbors[3],neighbors[4],neighbors[5]);*/
	fflush(stdout);	

	//init_sprng(1,time(0),SPRNG_DEFAULT);
	srand((unsigned int)time(NULL));
		
	// Open the initial condition (checkpoint or not) ----------------------

	if ( checkpoint_resume ) {
		file_open_error = 
		MPI_File_open(MPI_COMM_WORLD, CHECKPOINT, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
		MPI_File_set_view(fh,0, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
	}
	else {
		file_open_error = 
		MPI_File_open(MPI_COMM_WORLD, INITIAL, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
		MPI_File_set_view(fh,0, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
	}
	if (file_open_error != MPI_SUCCESS) {
		if (checkpoint_resume)
			fputs(CHECKPOINT, stderr);
		else
			fputs(INITIAL, stderr);
		fputs(" could not be opened.\n", stderr);
		exit(EXIT_FAILURE);
	}

	// Allocate and Populate the local array ----------------------------------
	
	local_array_copy_pointer = (char *)   malloc(mem_size[0] * mem_size[1] * mem_size[2] * sizeof(char));
	local_array_copy_2nd     = (char **)  malloc(mem_size[0] * mem_size[1] * sizeof(char*));
	local_array_copy         = (char ***) malloc(mem_size[0] * sizeof(char*));
	for(i = 0; i < mem_size[0] * mem_size[1]; i++)
		local_array_copy_2nd[i] = &local_array_copy_pointer[i * mem_size[2]];
	for(i = 0; i < mem_size[0]; i++)
		local_array_copy[i] = &local_array_copy_2nd[i * mem_size[1]];

	local_array_pointer = (char *)   malloc(mem_size[0] * mem_size[1] * mem_size[2] * sizeof(char));
	local_array_2nd  	= (char **)  malloc(mem_size[0] * mem_size[1] * sizeof(char*));
	local_array			= (char ***) malloc(mem_size[0] * sizeof(char*));
	for(i = 0; i < mem_size[0] * mem_size[1]; i++)
		local_array_2nd[i] = &local_array_pointer[i * mem_size[2]];
	for(i = 0; i < mem_size[0]; i++)
		local_array[i] = &local_array_2nd[i * mem_size[1]];
	
	// if (rank==0) printf("Malloc complete\n");
	
	for(row=0; row<mem_size[0]; row++) {
		for(col=0; col<mem_size[1]; col++) {
			for(pln=0; pln<mem_size[2]; pln++) {
				local_array[row][col][pln] = local_array_copy[row][col][pln] = '0';
			}
		}
	}
	
	// if (rank==0) printf("Setup complete\n");

	MPI_File_read_all(fh, local_array_pointer, 1, memtype, &status);

	if (rank==0) printf("File Read\n");
	
//	if (rank==0) {
//	for(row=0; row<mem_size[0]; row++) {
//		for(col=0; col<mem_size[1]; col++) {
//			for(pln=0; pln<mem_size[2]; pln++) {
//				printf("%c", local_array[row][col][pln]);
//			}
//			printf("\n");
//		}
//		printf("-----------------------\n");
//	}
//	}
	
	MPI_File_close(&fh);
		
	// Construct the plane data types
	
	MPI_Datatype yzplane;
	MPI_Type_vector(local_size[1], local_size[2], local_size[2]+2, MPI_CHAR, &yzplane);
	MPI_Type_commit(&yzplane);

	MPI_Datatype xzplane;
	MPI_Type_vector(local_size[0], local_size[2], ((local_size[2]+2)*local_size[1])+((local_size[2]+2)*2), MPI_CHAR, &xzplane);
	MPI_Type_commit(&xzplane);

	// this type will also copy the corner x columns, can't skip blocks intermittently
	// since we aren't worrying about the corner data, it's ok
	MPI_Datatype xyplane; 
	MPI_Type_vector((local_size[0]*local_size[1])+((local_size[0]*2)-2), 1, local_size[2]+2, MPI_CHAR, &xyplane);
	MPI_Type_commit(&xyplane);
					
	MPI_Barrier(comm);
	
	// start the iteration loop
	
	int iterations;
	int kCounter = k;
	for (iterations = 0; iterations < j; iterations++) {

		// send updated planes
		// Neighbors Array:  
		// 0     1     2     3     4       5
		// row-  col-  col+  row+  plane-  plane+
		// Note: corners are not handled
		
		// send top yzplane
		if (rank != neighbors[0]) MPI_Send(&local_array[1][1][1], 1, yzplane, neighbors[0], 0, comm);
		// recv bottom yzplane
		if (rank != neighbors[3]) MPI_Recv(&local_array[local_size[0]+1][1][1], 1, yzplane, neighbors[3], 0, comm, &status);

		// send bottom yzplane
		if (rank != neighbors[3]) MPI_Send(&local_array[local_size[0]][1][1], 1, yzplane, neighbors[3], 0, comm);
		// recv top yzplane
		if (rank != neighbors[0]) MPI_Recv(&local_array[0][1][1], 1, yzplane, neighbors[0], 0, comm, &status);

		// send left xzplane
		if (rank != neighbors[1]) MPI_Send(&local_array[1][1][1], 1, xzplane, neighbors[1], 0, comm);
		// recv right xzplane
		if (rank != neighbors[2]) MPI_Recv(&local_array[1][local_size[1]+1][1], 1, xzplane, neighbors[2], 0, comm, &status);

		// send right xzplane
		if (rank != neighbors[2]) MPI_Send(&local_array[1][local_size[1]][1], 1, xzplane, neighbors[2], 0, comm);
		// recv left xzplane
		if (rank != neighbors[1]) MPI_Recv(&local_array[1][0][1], 1, xzplane, neighbors[1], 0, comm, &status);

		// send front xyplane
		if (rank != neighbors[4]) MPI_Send(&local_array[1][1][1], 1, xyplane, neighbors[4], 0, comm);
		// recv back xyplane
		if (rank != neighbors[5]) MPI_Recv(&local_array[1][1][local_size[2]+1], 1, xyplane, neighbors[5], 0, comm, &status);

		// send back xyplane
		if (rank != neighbors[5]) MPI_Send(&local_array[1][1][local_size[2]], 1, xyplane, neighbors[5], 0, comm);
		// recv front xyplane
		if (rank != neighbors[4]) MPI_Recv(&local_array[1][1][0], 1, xyplane, neighbors[4], 0, comm, &status);

//		if (rank==0) {
//		for(row=0; row<mem_size[0]; row++) {
//			for(col=0; col<mem_size[1]; col++) {
//				for(pln=0; pln<mem_size[2]; pln++) {
//					printf("%c", local_array[row][col][pln]);
//				}
//				printf("\n");
//			}
//			printf("-----------------------\n");
//		}
//		}
		
		// run the game of life
		
		// gameOfLife(local_array, local_array_copy, local_size[0], local_size[1], l, rank);
		
		// swap the arrays
		
//		temp1 = local_array;
//		local_array = local_array_copy;
//		local_array_copy = temp1;
//
//		temp2 = local_array_pointer;
//		local_array_pointer = local_array_copy_pointer;
//		local_array_copy_pointer = temp2;		
	
		// check to see if this iteration needs a checkpoint
		
		kCounter--;
		if (kCounter == 0) {
			kCounter = k;
			
			// checkpoint code
			
			MPI_File_open(MPI_COMM_WORLD, CHECKPOINT, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
			MPI_File_set_view(fh, 0, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
			
			MPI_File_write_all(fh, local_array_pointer, 1, memtype, &status);
			
			MPI_File_close(&fh); 	

			if (rank == 0)
				printf("Checkpoint made: Iteration %d\n", iterations+1);
			
		} // end if kCounter == 0 
	} // end iteration loop	
	iterations--;
	
	// all done! repeat the checkpoint process
	
	MPI_File_open(MPI_COMM_WORLD, FINAL_RESULTS, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, filetype, "native", MPI_INFO_NULL);
	
	MPI_File_write_all(fh, local_array_pointer, 1, memtype, &status);
	
	MPI_File_close(&fh); 	

	if (rank == 0)
		printf("Final Results made: Iteration %d\n", iterations+1);
	
	time2 = MPI_Wtime();
	if (rank == 0)
	    printf("Elapsed Seconds: %f\n", time2-time1);fflush(stdout);
	
	MPI_Finalize(); 
	return EXIT_SUCCESS; 
}
