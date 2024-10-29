#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define the garden cell structure
struct garden{
    int deg;  // Degree requirement for this cell
    int isEmpty;  // Flag to indicate if the cell is empty (1) or occupied (0)
}typedef Garden;

void generate(Garden** garden,int row,int col,int num);

void generate(Garden** garden,int row,int col,int num)
{
    srand((unsigned)time(NULL));
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            garden[i][j].deg = 0;
            garden[i][j].isEmpty = 1;
        }
    }
    int remain = num;
    int r,c;
    int direc[4]; // up down left right
    while(1)
    {
START:
        if(remain == 0)break;
RA:
        r = rand() % row;
        c = rand() % col;

        if(garden[r][c].deg != 0)goto RA;

        if(r == 0)direc[0] = 0;
        else direc[0] = rand() % 2;
        if(r == row-1)direc[1] = 0;
        else direc[1] = rand() % 2;
        if(c == 0)direc[2] = 0;
        else direc[2] = rand() % 2;
        if(c == col-1)direc[3] = 0;
        else direc[3] = rand() % 2;

        if(direc[0] + direc[1] + direc[2] + direc[3] == 0)goto RA;
        //generate ain't for free!
        garden[r][c].isEmpty = 0;

        printf("generate:(%d,%d) degree:(%d,%d,%d,%d)\n",r,c,direc[0],direc[1],direc[2],direc[3]);

        remain -- ;
        garden[r][c].deg = direc[0] + direc[1] + direc[2] + direc[3];

        if(remain == 0)goto START;
        
        if(direc[0] == 1)
        {
            int x=0,y;
            for(int i=r-1;i>=0;i--)
            {
                if(garden[i][c].isEmpty == 0)
                {
                    x = i;
                    break;
                }
            }
            y = rand() % (r - x) + x;
            if(garden[y][c].deg > 0)garden[y][c].deg ++;
            else if (garden[y][c].isEmpty == 1)
            {
                garden[y][c].deg = 1;
                remain --;
            }
            else
            {
                garden[y][c].deg = 3;
                remain --;
            }
            for(int i=y;i<r;i++)
            {
                garden[i][c].isEmpty = 0;
            }

            printf("generate:(%d,%d)\n",y,c);
        }

        if(remain == 0)goto START;

        if(direc[1] == 1)
        {
            int x=row-1,y;
            for(int i=r+1;i<row;i++)
            {
                if(garden[i][c].isEmpty == 0)
                {
                    x = i;
                    break;
                }
            }
            y = rand() % (x - r) + r + 1;
            if(garden[y][c].deg > 0)garden[y][c].deg ++;
            else if (garden[y][c].isEmpty == 1)
            {
                garden[y][c].deg = 1;
                remain --;
            }
            else
            {
                garden[y][c].deg = 3;
                remain --;
            }
            for(int i=r+1;i<y;i++)
            {
                garden[i][c].isEmpty = 0;
            }

            printf("generate:(%d,%d)\n",y,c);
        }

        if(remain == 0)goto START;

        if(direc[2] == 1)
        {
            int x=0,y;
            for(int i=c-1;i>=0;i--)
            {
                if(garden[r][i].isEmpty == 0)
                {
                    x = i;
                    break;
                }
            }
            y = rand() % (c - x) + x;
            if(garden[r][y].deg > 0)garden[r][y].deg ++;
            else if (garden[r][y].isEmpty == 1)
            {
                garden[r][y].deg = 1;
                remain --;
            }
            else
            {
                garden[r][y].deg = 3;
                remain --;
            }
            for(int i=y;i<c;i++)
            {
                garden[r][i].isEmpty = 0;
            }

            printf("generate:(%d,%d)\n",r,y);
        }

        if(remain == 0)goto START;

        if(direc[3] == 1)
        {
            int x=col-1,y;
            for(int i=c+1;i<col;i++)
            {
                if(garden[r][i].isEmpty == 0)
                {
                    x = i;
                    break;
                }
            }
            y = rand() % (x - c) + c + 1;
            if(garden[r][y].deg > 0)garden[r][y].deg ++;
            else if (garden[r][y].isEmpty == 1)
            {
                garden[r][y].deg = 1;
                remain --;
            }
            else
            {
                garden[r][y].deg = 3;
                remain --;
            }
            for(int i=c;i<y;i++)
            {
                garden[r][i].isEmpty = 0;
            }

            printf("generate:(%d,%d)\n",r,y);
        }
        
        for(int i=0;i<row;i++)
        {
            for(int j=0;j<col;j++)
            {
                printf("%d ",garden[i][j].deg);
            }
            printf("\n");
        }

    }
}

// int main()
// {
//     int row,col;
//     row = 10;
//     col = 10;
//     int num = 10;
//     Garden** garden = (Garden**)malloc(sizeof(Garden*) * row);

//     for (int i = 0; i < row; i++) {
//         garden[i] = (Garden*)malloc(sizeof(Garden) * col);
//     }

//     generate(garden,row,col,num);


//     FILE *file = fopen("output.txt", "w");
//     if (file == NULL) {
//         perror("Error opening file");
//         return 1;
//     }

//     for(int i=0;i<row;i++)
//     {
//         for(int j=0;j<col;j++)
//         {
//             fprintf(file,"%d ",garden[i][j]);
//         }
//         fprintf(file,"\n");
//     }

//     fprintf(file,"------------------------\n");
//     for(int i=0;i<row;i++)
//     {
//         for(int j=0;j<col;j++)
//         {
//             if(garden[i][j].isEmpty == 0)fprintf(file,"# ");
//             else fprintf(file,"   ");
//         }
//         fprintf(file,"\n");
//     }
// }
