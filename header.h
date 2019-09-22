#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


// variaveis globais
int iterations, itt;
int height, width;
int *life, *lifeN, *aux;
int rows_thread, rows_last_thread; // divisao de linhas por thread
int num_threads;
pthread_barrier_t barrier;

// funcoes
int* createBoard();
void freeBoard(int *board);
void initBoard(int *board, char *file_name);
void showBoard(int *board);
void core(int row_start, int row_end);
void copyBoard(int *board, int *copy);
void run(int row_start, int row_end, int id);
void* gameOfLife(void* arg);
