#include <stdio.h>
#include <stdlib.h>

struct squarenode {
    // location of a square
    int row;  
    int col;
    // num of connectors in one square
    int deg;
    // 1 if there is a connector in that direction(in the order of up, down, left and right)
    int direc[4];
}typedef node;


int backtrack(int** garden, int r, int c, int row, int col, node** connector);
int main() {
    int r,c, num = 0, capacity = 20;
    scanf("%d %d",&r,&c);
    int** garden = (int**)malloc(sizeof(int*) * r);
    node* connector = (node*)malloc(sizeof(node) * capacity);
    for(int i=0;i<r;i++) //read garden 
    {
        garden[i] = (int*)malloc(sizeof(int) * c);
        for(int j=0;j<c;j++)
        {
            scanf("%d",&garden[i][j]);
            if(garden[i][j] != 0){
                if(num + 1 > capacity){
                    capacity *= 2;
                    connector = (node*)realloc(connector, capacity * sizeof(node));
                }
                connector[num].row = i;
                connector[num].col = j;
                connector[num++].deg = garden[i][j];
            }
        }
    }
    if(backtrack(garden, r, c, 0, 0, connector)){
        for(int i = 0; i < num ; i++){
            printf("%d %d %d %d %d %d\n", connector[i].row, connector[i].col, connector[i].direc[0], connector[i].direc[1], connector[i].direc[2], connector[i].direc[3]);
        }
    }
    else{
        printf("No Solution");
    }
    return 0;
}
