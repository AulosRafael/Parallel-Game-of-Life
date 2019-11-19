#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <time.h>


// prototype
int* createBoard(int height, int width);
void freeBoard(int *grid);
int* createInitBoard(char *file_name, int *height, int *width);
void showBoard(int *grid, int height, int width);
void core(int *life, int *lifeN, int row_start, int row_end, int height, int width);

void run(int **grid, int height, int width, int iterations, int pid, int np);
int* createInitSubGrid(int *grid, int height, int width, int *height_sub_grid, int np, int root, int my_pid);
void merger(int *grid, int *sub_grid, int height, int width, int np, int root, int pid);
void gameOfLife(int *grid, int height, int width, int np, int root, int pid, int iterations);

		
void run(int **grid, int height, int width, int iterations, int pid, int np) {
	int *next_grid = createBoard(height, width);
	int *temp;

	int left_pid = (pid-1+np) % np;
	int right_pid = (pid+1) % np;

    for (int itt = 0; itt < iterations; itt++) {
		core(*grid, next_grid, 0, height, height, width);

		// enviando bordas para processos vizinhos
		MPI_Send(&next_grid[width], width, MPI_INT, left_pid, 0, MPI_COMM_WORLD);
		MPI_Send(&next_grid[(height-2)*width], width, MPI_INT, right_pid, 0, MPI_COMM_WORLD);

		// recebendo bordas dos processos vizinhos
		MPI_Recv(&next_grid[0], width, MPI_INT, left_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&next_grid[(height-1) * width], width, MPI_INT, right_pid, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		temp = next_grid;
		next_grid = *grid;
		*grid = temp;
	}

	freeBoard(next_grid);
}


int* createInitSubGrid(int *grid, int height, int width, int *height_sub_grid, int np, int root, int my_pid) {
	// numeros de linhas por processo
	int rows = height / np; 
	int rows_last = height % np;
	int id_start, id_end, id_up, id_down;
	int h_sg = (my_pid == np - 1) ? (rows + rows_last) : rows;
	*height_sub_grid = h_sg + 2;

	int *sub_grid = createBoard(*height_sub_grid, width);

	if (my_pid == root) {
		for (int pid = 0; pid < np; pid++) {
			id_start = pid * rows * width;
			id_end = id_start + rows * width;
			id_up = id_start - width;
			id_down = id_end;

			if (pid == np - 1) {
				id_end += rows_last * width;
				id_down = 0;
			}

			if (pid == root) {
				id_up = ((np-1) * rows + rows_last) * width;

				int i, j;
				for (i = 0, j = 0; i < width; i++, j++) {
					sub_grid[i] = grid[id_up + j];
					sub_grid[id_end - id_start + width + i] = grid[id_down + j];					
				}

				for (i = 0, j = 0; i < id_end - id_start; i++, j++) 
					sub_grid[width + i] = grid[id_start + j];
			}

			else {  // pid != root
				MPI_Send(&grid[id_up], width, MPI_INT, pid, 0, MPI_COMM_WORLD); 				// line up
				MPI_Send(&grid[id_start], id_end - id_start, MPI_INT, pid, 0, MPI_COMM_WORLD);	// sub_grid
				MPI_Send(&grid[id_down], width, MPI_INT, pid, 0, MPI_COMM_WORLD);				// line down
			}
		}
	}

	else {
		MPI_Recv(&sub_grid[0], width, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);				// line up
		MPI_Recv(&sub_grid[width], h_sg * width, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	// sub_grid
		MPI_Recv(&sub_grid[(h_sg+1) * width], width, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	// line down
	}

	return sub_grid;
}


void merger(int *grid, int *sub_grid, int height, int width, int np, int root, int pid) {
	int rows = height / np;
	int rows_last = height % np;
	int h_sg;

	if (pid != root) {
		h_sg = (pid == np - 1) ? (rows + rows_last) : rows;
		MPI_Send(&sub_grid[width], h_sg * width, MPI_INT, root, 0, MPI_COMM_WORLD);
	}
	
	else { // pid == 0
		for (int p = 0; p < np; p++) {
			h_sg = (p == np - 1) ? (rows + rows_last) : rows;

			// tentar usar MPI_Gatherv
			if (p != root)
				MPI_Recv(&grid[rows * p * width], h_sg * width, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			
			else
				for (int i = 0; i < h_sg * width; i++)
					grid[rows * p * width + i] = sub_grid[width + i];
		}
	}
}


void gameOfLife(int *grid, int height, int width, int np, int root, int pid, int iterations) {
	int *sub_grid;
	int height_sub_grid;

	MPI_Bcast(&height, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&width, 1, MPI_INT, root, MPI_COMM_WORLD);
	
	sub_grid = createInitSubGrid(grid, height, width, &height_sub_grid, np, root, pid);
	run(&sub_grid, height_sub_grid, width, iterations, pid, np);
	merger(grid, sub_grid, height, width, np, root, pid);

	freeBoard(sub_grid);
}


//Executes the Game of Life Algorithim showing the initial and final grids
// arg 1 = number of iterations
// arg 2 = file input grid

int main(int argc, char** argv) {
    if (argc < 3) exit(0);
   
    clock_t ti, tf;
    int iterations = atoi(argv[1]);
    char *file_name = argv[2];

    // root nao pode ser ultimo processo, pq o ultimo processo revebe linhas extras
	int pid, np, root = 0;
	int height, width;
	int *grid;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &np);

	if (pid == root) {
		grid = createInitBoard(file_name, &height, &width);
		// showBoard(grid, height, width);
	}

    ti = clock();

    gameOfLife(grid, height, width, np, root, pid, iterations);

    tf = clock();

	if (pid == root) {
	    printf("%lf\n", ((double)tf-ti) / CLOCKS_PER_SEC);
		// showBoard(grid, height, width);
		freeBoard(grid);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}
