#include "header.h"


//Gets 1-dimension position in array given 2-dimension positions and the matrix width
int pos(int i,int j,int y) {
    return i*y+j;
}


//Checks if given cell is alive according to Conway's Game of Life rules(note that the board is circular on the edges)
int alive(int* life, int i, int j, int x, int y){
    int u = pos(i - 1 < 0 ? x-1 : i - 1, j, y);
    int d = pos(i + 1 > x-1 ? 0 : i + 1, j, y);
    int l = pos(i, j - 1 < 0 ? y-1 : j - 1, y);
    int r = pos(i, j + 1 > y-1 ? 0 : j + 1, y);
    int ul = pos(i - 1 < 0 ? x-1 : i - 1,  j - 1 < 0 ? y-1 : j - 1, y);
    int ur = pos(i - 1 < 0 ? x-1 : i - 1,  j + 1 > y-1 ? 0 : j + 1, y);
    int dl = pos(i + 1 > x-1 ? 0 : i + 1,  j - 1 < 0 ? y-1 : j - 1, y);
    int dr = pos(i + 1 > x-1 ? 0 : i + 1,  j + 1 > y-1 ? 0 : j + 1, y);
    int count = life[u] + life[d] +  life[l] + life[r] + life[ul] + life[ur] + life[dl] + life[dr];
    return (!life[pos(i,j,y)] && count == 3) || (life[pos(i,j,y)] && count > 1 && count < 4);
}


int* createBoard() {
    int *board = malloc(height * width * sizeof(int));
    return board;
}


void freeBoard(int *board) {
    free(board);
}


void initBoard(int *board, char *file_name) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }

    int i = 0;
    char c;
    while (i < height * width) {
        c = fgetc(file);
        if (c == '.' || c == 'x')
            board[i++] = c=='x';            
    }

    fclose(file);
}


//Prints the board in a "beautiful" way
void showBoard(int *board) {
    printf("Board: %d x %d\n", height, width);
    puts("------------------------------");
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            printf("%d", board[pos(i,j,width)]);
        }
        puts(" ");
    }
    puts("------------------------------");
}


void core(int row_start, int row_end) {
    for(int i = row_start; i < row_end; i++)
        for(int j = 0; j < width; j++)
            lifeN[pos(i,j,width)] = alive(life, i, j, height, width);
}
