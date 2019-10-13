/******************************
* Created by: Márcio Medeiros *
* in: 20/08/2019              *
******************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// prototype
int* createBoard(int height, int width);
void freeBoard(int *board);
int* createInitBoard(char *file_name, int *height, int *width);
void showBoard(int *board, int height, int width);
void core(int *life, int *lifeN, int row_start, int row_end, int height, int width);


//Executes the Game of Life Algorithim showing the initial and final boards
// arg 1 = number of iterations
// arg 2 = file input board

int main(int argc, char **argv) {
    if (argc < 3) exit(0);
    clock_t ti, tf;

    int height;
    int width;
    int iterations = atoi(argv[1]);
    char *file_name = argv[2];

    int *life = createInitBoard(file_name, &height, &width);
    int *lifeN = createBoard(height, width);
    int *aux;
    
    ti = clock();

    for (int i = 0; i < iterations; i++) {
        core(life, lifeN, 0, height, height, width);
        
        // copy new board
        aux = lifeN;
        lifeN = life;
        life = aux;
    }

    tf = clock();
    // showBoard(life, height, width);
    // printf("Sequencial\n%dx%d board\n%d iteracoes\n", height, width, iterations);
    printf("%lf\n", ((double)tf-ti) / CLOCKS_PER_SEC);

    freeBoard(life);
    freeBoard(lifeN);
    return 0;
}
