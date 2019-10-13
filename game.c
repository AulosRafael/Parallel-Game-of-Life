#include <stdio.h>
#include <stdlib.h>


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


int* createBoard(int height, int width) {
    int *board = malloc(height * width * sizeof(int));
    return board;
}


void freeBoard(int *board) {
    free(board);
}


int* createInitBoard(char *file_name, int *height, int *width) {    
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Cannot open file \n");
        exit(0);
    }

    int *board = (int*) malloc(1);
    int lin = 1, size = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '.' || c == 'x') {
            size++;
            board = (int*) realloc(board, size * sizeof(int));
            board[size-1] = c=='x';
        }

        else if (c == '\n') {
            lin++;
        }
    }

    *height = lin; 
    *width = size / lin;
    fclose(file);
    return board;
}


void core(int *life, int *lifeN, int row_start, int row_end, int height, int width) {
    for(int i = row_start; i < row_end; i++)
        for(int j = 0; j < width; j++)
            lifeN[pos(i,j,width)] = alive(life, i, j, height, width);
}


//Prints the board in a "beautiful" way
void showBoard(int *board, int height, int width) {
    // system("@cls||clear");
    printf("Board: %d x %d\n", height, width);
    
    int i;
    for (int i = 0; i < width; i++)
        printf("+");
    puts("");

    for(i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if (board[pos(i,j,width)] == 0)
                printf(" ");
            else
                printf("o");
        }
        puts("|");
    }

    for (int i = 0; i < width; i++)
        printf("+");
    puts("|");
}
