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
void initBoard(int *board, char *file_name, int height, int width);
void showBoard(int *board, int height, int width);
void core(int *life, int *lifeN, int row_start, int row_end, int height, int width);


//Executes the Game of Life Algorithim showing the initial and final boards
// arg 1 = boards height
// arg 2 = boards width
// arg 3 = number of iterations
// arg 4 = file input board

int main(int argc, char **argv) {
    if (argc < 5) exit(0);
    clock_t ti, tf;

    int height = atoi(argv[1]);
    int width = atoi(argv[2]);
    int iterations = atoi(argv[3]);
    char *file_name = argv[4];

    int *life = createBoard(height, width);
    int *lifeN = createBoard(height, width);
    int *aux;
    initBoard(life, file_name, height, width);
    // showBoard(life, height, width);

    ti = clock();

    for (int i = 0; i < iterations; i++) {
        core(life, lifeN, 0, height, height, width);

        // copy new board
        aux = lifeN;
        lifeN = life;
        life = aux;
    }


    tf = clock();
    printf("Tempo de execucao: %lf", ((double)tf-ti)/((CLOCKS_PER_SEC/1000)));

    // showBoard(life, height, width);

    freeBoard(life);
    freeBoard(lifeN);
    return 0;
}
