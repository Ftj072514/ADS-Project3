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

int main() {
    int r,c;
    scanf("%d %d",&r,&c);
    int garden[r][c];


    for(int i=0;i<r;i++) //read garden 
    {
        for(int j=0;j<c;j++)
        {
            scanf("%d",&garden[i][j]);
        }
    }

    int num=0;  // connector number

    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            if(garden[i][j] != 0)
            {
                num++;
            }
        }
    }

    node connector[num];

    int t=0;
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            if(garden[i][j] != 0)
            {
                //read connnector
                t++;
            }
        }
    }


    
}
