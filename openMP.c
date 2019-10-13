#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// variaveis globais
int iterations;
int height, width;
int *life, *lifeN, *aux;
int rows_thread, rows_last_thread; // divisao de linhas por thread
int thread_count;

// prototype
int* createBoard(int height, int width);
void freeBoard(int *board);
int* createInitBoard(char *file_name, int *height, int *width);
void showBoard(int *board, int height, int width);
void core(int *life, int *lifeN, int row_start, int row_end, int height, int width);
void run(int row_start, int row_end, int id);
void gameOfLife(void);


void run(int row_start, int row_end, int id) {
    int itt = 0;
    while (itt < iterations) {
        core(life, lifeN, row_start, row_end, height, width);

        // wait for all threads        
        #pragma omp barrier

        // copy boards
        if (id == 0) {
            aux = lifeN;
            lifeN = life;
            life = aux;
            itt++;
        }

        // wait for all threads        
        #pragma omp barrier
    }
}

void gameOfLife(void) {
    int id = omp_get_thread_num();
    int row_start = id * rows_thread;
    int row_end = row_start + rows_thread;

    if (id == thread_count - 1)
        row_end += rows_last_thread;

    run(row_start, row_end, id);
}


//Executes the Game of Life Algorithim showing the initial and final boards
// arg 1 = number of iterations
// arg 2 = file input board
// arg 3 = number of threads

int main(int argc, char **argv) {
    if (argc < 4) exit(0);
    clock_t ti, tf;

    iterations = atoi(argv[1]);
    char *file_name = argv[2];
    thread_count = atoi(argv[3]);
    
    life = createInitBoard(file_name, &height, &width);
    lifeN = createBoard(height, width);

    // divisão das linhas por thread
    rows_thread = height / thread_count; 
    rows_last_thread = height % thread_count; //linhas para ultima thread

    ti = clock();

    #pragma omp parallel num_threads(thread_count)
        gameOfLife();

    tf = clock();
    // showBoard(life, height, width);
    // printf("Pthread\n%d threads\n%dx%d board\n%d iteracoes\n", num_threads, height, width, iterations);
    printf("%lf\n", ((double)tf-ti) / CLOCKS_PER_SEC);

    freeBoard(life);
    freeBoard(lifeN);
    return 0;
}
