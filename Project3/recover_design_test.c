#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define col 40
#define row 40

void process(int** garden,int** g,int i,int j);

int main()
{
    srand((unsigned)time(NULL));
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    int** garden = (int**)malloc(sizeof(int*) * row);
    for(int i=0;i<row;i++)
        garden[i] = (int*)malloc(sizeof(int) * col);

    fprintf(file,"%d %d\n",row,col);

    int x;
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            x = (rand() % 4) + 1;
            if(x == 1)garden[i][j] = 1;
            else garden[i][j] = 0;
        }
    }



    int** g = (int**)malloc(sizeof(int*) * row);
    for(int i=0;i<row;i++)
        g[i] = (int*)malloc(sizeof(int) * col);

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            process(garden,g,i,j);
            fprintf(file,"%d ",g[i][j]);
        }
        fprintf(file,"\n");
    }


    fprintf(file,"\n\n--------------------------------\n\nhint:\n");
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(garden[i][j] == 0)fprintf(file,"#");
            else fprintf(file," ");
        }
        fprintf(file,"\n");
    }
    fclose(file);
}

void process(int** garden,int** g,int i,int j)
{
    int degree;

    if(garden[i][j] == 0)
    {
        g[i][j] = 0;
        return ;
    }

    if(i == 0)
    {
        if(j == 0)
        {
            g[i][j] = garden[1][0] + garden[0][1];
        }
        else if(j == col - 1)
        {
            g[i][j] = garden[0][col-2] + garden[1][col-1];
        }
        else
        {
            degree = garden[0][j-1] + garden[0][j+1] + garden[1][j]; //degree
            switch(degree)
            {
                case 0: 
                {
                    g[i][j] = 0;
                    break;
                }
                case 1: 
                {
                    g[i][j] = 1;
                    break;
                }
                case 2:
                {
                    if(garden[1][j] == 0)g[i][j] = 0;
                    else g[i][j] = 2;
                    break;
                }
                case 3:
                {
                    g[i][j] = 3;
                    break;
                }
            }
        }
    }
    else if(i == row-1)
    {
        if(j == 0)
        {
            g[i][j] = garden[row-2][0] + garden[row-1][1];
        }
        else if(j == col - 1)
        {
            g[i][j] = garden[row-2][col-1] + garden[row-1][col-2];
        }
        else
        {
            degree = garden[row-1][j-1] + garden[row-1][j+1] + garden[row-2][j];
            switch(degree)
            {
                case 0: 
                {
                    g[i][j] = 0;
                    break;
                }
                case 1: 
                {
                    g[i][j] = 1;
                    break;
                }
                case 2:
                {
                    if(garden[row-2][j] == 0)g[i][j] = 0;
                    else g[i][j] = 2;
                    break;
                }
                case 3:
                {
                    g[i][j] = 3;
                    break;
                }
            }
        }
    }
    else if (j == 0)
    {
        degree = garden[i-1][0] + garden[i+1][0] + garden[i][1];
        switch(degree)
            {
                case 0: 
                {
                    g[i][j] = 0;
                    break;
                }
                case 1: 
                {
                    g[i][j] = 1;
                    break;
                }
                case 2:
                {
                    if(garden[i][1] == 0)g[i][j] = 0;
                    else g[i][j] = 2;
                    break;
                }
                case 3:
                {
                    g[i][j] = 3;
                    break;
                }
            }
    }
    else if (j == col - 1)
    {
        degree = garden[i-1][col-1] + garden[i+1][col-1] + garden[i][col-2];
        switch(degree)
            {
                case 0: 
                {
                    g[i][j] = 0;
                    break;
                }
                case 1: 
                {
                    g[i][j] = 1;
                    break;
                }
                case 2:
                {
                    if(garden[i][col-2] == 0)g[i][j] = 0;
                    else g[i][j] = 2;
                    break;
                }
                case 3:
                {
                    g[i][j] = 3;
                    break;
                }
            }
    }
    else
    {
        degree = garden[i-1][j] + garden[i+1][j] + garden[i][j-1] + garden[i][j+1];
        switch(degree)
            {
                case 0: 
                {
                    g[i][j] = 0;
                    break;
                }
                case 1: 
                {
                    g[i][j] = 1;
                    break;
                }
                case 2:
                {
                    if(garden[i-1][j] == garden[i+1][j])g[i][j] = 0;
                    else g[i][j] = 2;
                    break;
                }
                case 3:
                {
                    g[i][j] = 3;
                    break;
                }
                case 4:
                {
                    g[i][j] = 4;
                    break;
                }
            }
    
    }   
}
