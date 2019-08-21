#include <stdio.h>
#include <stdlib.h>

int pos(int i,int j,int y){
    return i*y+j;
}

void show(int* board, int x, int y){
    puts("------------------------------");
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            printf("%d",board[pos(i,j,y)]);
        }
        puts(" ");
    }
    puts("------------------------------");
}

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

void copyBoard(int* a, int*b, int size){
    for(int i = 0; i < size; i++){
        a[i] = b[i];
    }
}

int main(int argc, char **argv)
{
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    int repeats = atoi(argv[3]);
    
    int life[x*y];
    int lifeN[x*y];
    
    
    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            life[pos(i,j,y)] = 0;
            if(i==1 && (j == 3 || j == 4 || j == 5)){
                life[pos(i,j,y)] = 1;
            }
        }
    }
    copyBoard(lifeN, life, x*y);
    show(life,x,y);
    
    for(int z = 0; z < repeats; z++){
        for(int i = 0; i < x; i++){
            for(int j = 0; j < y; j++){
                lifeN[pos(i,j,y)] = alive(life, i, j, x, y);
            }
        }
        copyBoard(life, lifeN, x*y);
        show(life,x,y);
    }

    show(life, x, y);

    return 0;
}
