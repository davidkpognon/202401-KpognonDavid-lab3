#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int* worker_validation;

int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** board = (int**)malloc(sizeof(int*) * ROW_SIZE);
    
    for (int row = 0; row < ROW_SIZE; row++) {
        board[row] = (int*)malloc(sizeof(int) * COL_SIZE);
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int d = 0; d < ROW_SIZE; d++) {
        for (int r = 0; r < COL_SIZE; r++) {
            fscanf(fp, "%d%*c", &board[d][r]);
        }
    }   

    fclose(fp);
    return board;
}