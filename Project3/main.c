#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct connector {
    // location of a square
    int row;  
    int col;
    // num of connectors in one square
    int deg;
    int rem_deg;
    // 1 if there is a connector in that direction(in the order of up, down, left and right)
    int direc[4];
}typedef node;

struct garden{
    int deg;
    int isEmpty;
}typedef Garden;

int all_valid_connections(Garden** garden,int r,int c,int row,int col,node* connector,int** connections, int size);
int is_valid(int* connection, Garden** garden, int r, int c, int row, int col, node* connector, int size, int num);
int backtrack(Garden** garden, int r, int c, int row, int col, node* connector, int size, int numofConnec);
void undo(Garden** garden, int num, node* connector, int i, int size, int numofConnec);

int main() {
    int r,c, num = 0, capacity = 20;
    scanf("%d %d",&r,&c);
    Garden** garden = (Garden**)malloc(sizeof(Garden*) * r);
    node* connector = (node*)malloc(sizeof(node) * capacity);
    for(int i=0;i<r;i++) //read garden 
    {
        garden[i] = (Garden*)malloc(sizeof(Garden) * c);
        for(int j=0;j<c;j++)
        {
            scanf("%d",&garden[i][j].deg);
            garden[i][j].isEmpty = 1;
            if(garden[i][j].deg != 0){
                garden[i][j].isEmpty = 0;
                if(num + 1 > capacity){
                    capacity *= 2;
                    connector = (node*)realloc(connector, capacity * sizeof(node));
                }
                connector[num].row = i;
                connector[num].col = j;
                connector[num].deg = garden[i][j].deg;
                connector[num].rem_deg = garden[i][j].deg;
                memset(connector[num++].direc, 0, sizeof(int) * 4);
            }
        }
    }
    if(backtrack(garden, r, c, 0, 0, connector, 0, num)){
        for(int i = 0; i < num ; i++){
            printf("%d %d %d %d %d %d\n", connector[i].row + 1, connector[i].col + 1, connector[i].direc[0], connector[i].direc[1], connector[i].direc[2], connector[i].direc[3]);
        }
    }
    else{
        printf("No Solution");
    }
    return 0;
}

int backtrack(Garden** garden, int r, int c, int row, int col, node* connector, int size, int numofConnec) {
    int flag;
    if(row == r)  //every row has been processed
        return 1;
    int next_row = row, next_col = col + 1;
    if(next_col == c){  //all columns in this row have been checked
        next_col = 0;
        next_row = row + 1;
    }
    if(garden[row][col].deg == 0)
        return backtrack(garden, r, c, next_row, next_col, connector, size, numofConnec); //if this node is not a connector
    else if(connector[size].rem_deg == 0)
        return backtrack(garden, r, c, next_row, next_col, connector, size + 1, numofConnec);
    
    int num; //number of possible combinations 
    switch(connector[size].rem_deg){
        case 1: num = 2; break;
        case 2: num = 1; break;
        case 3: return 0; 
        case 4: return 0;
    }
    int** connections = (int**)malloc(sizeof(int*) * num);  //store the possible direc[]
    for(int i = 0; i < num; i++)
        connections[i] = (int*)malloc(sizeof(int) * 4);
    all_valid_connections(garden, r, c, row, col, connector, connections, size);
    for(int i = 0; i < num; i++){
        printf("%d %d %d %d\n", connector[size].row, connector[size].col, connector[size].rem_deg, i);
        if(is_valid(connections[i], garden, r, c, row, col, connector, size, numofConnec)){
            if(backtrack(garden, r, c, next_row, next_col, connector, size + 1, numofConnec))
                return 1;
            else{
                undo(garden, num, connector, i, size, numofConnec);
            }
        }
    }
    return 0;
}

//the enumeration of connector is down and right ,the left and up connector are simply -1 or 0,and they out of enmeration
int all_valid_connections(Garden** garden,int r,int c,int row,int col,node* connector,int** connections, int size)
{
    int t = size;
    //connector[t] is exact connector

    if(connector[t].rem_deg>2)return 0;
    switch(connector[t].rem_deg){
        case 0: {
            connections[0][0] = connector[t].direc[0];
            connections[0][1] = 0;
            connections[0][2] = connector[t].direc[2];
            connections[0][3] = 0;
            return 1;
        }
        case 1: {
            for(int i=0;i<2;i++)
            {
                connections[i][0] = connector[t].direc[0];
                connections[i][1] = 1 - i;
                connections[i][2] = connector[t].direc[2];
                connections[i][3] = i;
            }
            return 1;
        }
        case 2: {
            connections[0][0] = connector[t].direc[0];
            connections[0][1] = 1;
            connections[0][2] = connector[t].direc[2];
            connections[0][3] = 1;
            return 1;
        }
    }
}

int is_valid(int* connection, Garden** garden, int r, int c, int row, int col, node* connector, int size, int num)
{
    if(col == c - 1 && connection[3] == 1 || row == r - 1 && connection[1] == 1 || col == 0 && connection[2] == 1 || row == 0 && connection[0] == 1){
        return 0;
    }
    int right, flag = 0;
    if(connection[3] == 1) //right direction
    {
        for(int i = size + 1; i < num; i++){
            if(connector[size].row == connector[i].row){
                for(int j = col + 1; j < connector[i].col; j++){
                    if(garden[row][j].isEmpty == 0){
                        printf("%d %d", row, j);
                        printf("error\n");
                        return 0;
                    }
                }
                if(connector[i].rem_deg == 0)
                    return 0;
                if(connection[1] == 0){
                    for(int j = col + 1; j < connector[i].col; j++){
                        garden[row][j].isEmpty = 0;
                    }
                    connector[size].rem_deg--;
                    connector[size].direc[3] = 1;
                    connector[i].rem_deg--;
                    connector[i].direc[2] = 1;
                    return 1;
                }
                else if(connection[1] == 1){
                    right = i;
                    flag = 1;
                    break;
                }
            }
        }
    }
    if(connection[1] == 1) //down direction
    {
        for(int i = size + 1; i < num; i++){
            if(connector[size].col == connector[i].col){
                for(int j = row + 1; j < connector[j].row; j++){
                    if(garden[j][col].isEmpty == 0)
                        return 0;
                }
                if(connector[i].rem_deg == 0)
                    return 0;
                for(int j = row + 1; j < connector[j].row; j++){
                    garden[j][col].isEmpty = 0;
                }
                connector[size].rem_deg--;
                connector[size].direc[1] = 1;
                connector[i].rem_deg--;
                connector[i].direc[0] = 1;
                if(flag){
                    for(int j = col + 1; j < connector[i].col; j++){
                        garden[row][j].isEmpty = 0;
                    }
                    connector[size].rem_deg--;
                    connector[size].direc[3] = 1;
                    connector[right].rem_deg--;
                    connector[right].direc[2] = 1;
                }
                return 1;
            }
        }
    }
    return 0;
}

void undo(Garden** garden, int num, node* connector, int i, int size, int numofConnec) {
    int flag = 1;
    if(num == 2 && i == 0){
        for(int j = size + 1; j < numofConnec; j++){
            if(connector[size].col == connector[j].col){
                for(int i = connector[size].row + 1; i < connector[j].row; i++)
                    garden[i][connector[size].col].isEmpty = 1;
                connector[size].rem_deg++;
                connector[size].direc[1] = 0;
                connector[j].rem_deg++;
                connector[j].direc[0] = 0;
                return;
            }
        }
    }
    else if(num == 2 && i == 1){
        for(int j = size + 1; j < numofConnec; j++){
            if(connector[size].row == connector[j].row){
                for(int i = connector[size].col + 1; i < connector[j].col; i++)
                    garden[connector[size].row][i].isEmpty = 1;
                connector[size].rem_deg++;
                connector[size].direc[3] = 0;
                connector[j].rem_deg++;
                connector[j].direc[2] = 0;
                return;
            }
        }
    }
    else{
        for(int j = size + 1; j < numofConnec; j++){
            if(connector[size].row == connector[j].row && flag){
                for(int i = connector[size].col + 1; i < connector[j].col; i++)
                    garden[connector[size].row][i].isEmpty = 1;
                connector[size].rem_deg++;
                connector[size].direc[3] = 0;
                connector[j].rem_deg++;
                connector[j].direc[2] = 0;
                flag = 0;
            }
            if(connector[size].col == connector[j].col){
                for(int i = connector[size].row + 1; i < connector[j].row; i++)
                    garden[i][connector[size].col].isEmpty = 1;
                connector[size].rem_deg++;
                connector[size].direc[1] = 0;
                connector[j].rem_deg++;
                connector[j].direc[0] = 0;
                return;
            }
        }
    }
}