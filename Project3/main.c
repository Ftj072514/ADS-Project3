#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct squarenode {
    // location of a square
    int row;  
    int col;
    // num of connectors in one square
    int deg;
    int rem_deg;
    // 1 if there is a connector in that direction(in the order of up, down, left and right)
    // -1 if there is a connector and it is fixed
    int direc[4];
}typedef node;


int backtrack(int** garden, int r, int c, int row, int col, node* connector, int size);
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
                connector[num].deg = garden[i][j];
                connector[num++].rem_deg = garden[i][j];
                memset(connector[num].direc, 0, sizeof(int) * 4);
            }
        }
    }
    if(backtrack(garden, r, c, 0, 0, connector, 0)){
        for(int i = 0; i < num ; i++){
            printf("%d %d %d %d %d %d\n", connector[i].row, connector[i].col, connector[i].direc[0], connector[i].direc[1], connector[i].direc[2], connector[i].direc[3]);
        }
    }
    else{
        printf("No Solution");
    }
    return 0;
}

int backtrack(int** garden, int r, int c, int row, int col, node* connector, int size) {
    if(row == r)  //every row has been processed
        return 1;
    int next_row = row, next_col = col + 1;
    if(next_col == c){  //all columns in this row have been checked
        next_col = 0;
        next_row = row + 1;
    }
    if(garden[row][col] == 0)
        return backtrack(garden, r, c, next_row, next_col, connector, size); //if this node is not a connector
    
    int num; //number of possible combinations 
    switch(garden[row][col]){
        case 1: 
        case 3: num = 4; break;   //C(4,1) = C(4,3) = 4
        case 2: num = 6; break;
        case 4: num = 1; break;
    }
    int** connections = (int**)malloc(sizeof(int*) * num);  //store the possible indexes of direc[]: if num=2 and we choose up and left, then return [0,2]
    for(int i = 0; i < num; i++)
        connections[i] = (int*)malloc(sizeof(int) * garden[row][col]);
    connections = all_valid_connections(garden, r, c, row, col);
    for(int i = 0; i < num; i++){
        if(is_valid(connections[i], garden, r, c, row, col, connector)){
            for(int j = 0; j < garden[row][col]; j++){
                connector[size].direc[connections[i][j]] = 1;
            }
            if(backtrack(garden, r, c, next_row, next_col, connector, ++size))
                return 1;
        }
    }
    return 0;
}