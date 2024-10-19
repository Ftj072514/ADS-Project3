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
    return 0;
}