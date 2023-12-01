#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int worker_validation;

//  read Sudoku board from file
int** read_board_from_file(char* filename) {
    // Open the file for reading
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // allocate memory for the 
    sudoku_board = (int**)malloc(sizeof(int*) * ROW_SIZE);
    for (int row = 0; row < ROW_SIZE; row++) {
        sudoku_board[row] = (int*)malloc(sizeof(int) * COL_SIZE);
        // read values from the file 
        for (int col = 0; col < COL_SIZE; col++) {
            // check if the expected number of values are read from the file
            if (fscanf(fp, "%d,", &sudoku_board[row][col]) != 1) {
                perror("Error reading from file");
                exit(EXIT_FAILURE);
            }
        }
    }

    // Close the file after reading
    fclose(fp);
    
    return sudoku_board;
}

