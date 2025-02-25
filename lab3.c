#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"

extern int** sudoku_board;
int worker_validation;
// David Kpognon code
// function to read Sudoku board from file
int** read_board_from_file(char* filename) {
    // Open the file for reading
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // allocate memory for the Sudoku board
    sudoku_board = (int**)malloc(sizeof(int*) * ROW_SIZE);
    for (int row = 0; row < ROW_SIZE; row++) {
        sudoku_board[row] = (int*)malloc(sizeof(int) * COL_SIZE);
        // read values from the file 
        for (int col = 0; col < COL_SIZE; col++) {
            // see if the expected number of values are read from the file
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


// function to validate a row, column, or square
void validateArray(void* param, int size, int (*getValue)(int, int, int, int)) {
    param_struct* arg = (param_struct*)param;
    int arr[ROW_SIZE] = {0};

    // Loops through the specified array 
    for (int i = 0; i < size; i++) {
        int current = getValue(arg->starting_row, arg->starting_col, i / 3, i % 3);
        // see if the value is valid
        if (current < 1 || current > 9 || arr[current - 1] == 1) {
            worker_validation = 0;
            return;
        }
        arr[current - 1]++;
    }

    // see if all values from 1 to 9 are present 
    for (int i = 0; i < 9; i++) {
        if (arr[i] != 1) {
            worker_validation = 0;
            return;
        }
    }

    // if all validations pass worker_validation to 1
    worker_validation = 1;
}

// validation function for a row
int getRowValue(int row, int col, int i, int j) {
    return sudoku_board[row][col + i];
}

void* validateRow(void* param) {
    validateArray(param, ROW_SIZE, getRowValue);
    return NULL;
}

// validation function for a column
int getColValue(int row, int col, int i, int j) {
    return sudoku_board[row + i][col];
}

void* validateCol(void* param) {
    validateArray(param, COL_SIZE, getColValue);
    return NULL;
}

// validation function for a square
int getSquareValue(int row, int col, int i, int j) {
    return sudoku_board[row + i][col + j];
}

void* validateSquare(void* param) {
    validateArray(param, 9, getSquareValue);
    return NULL;
}
//valid
int is_board_valid() {
    pthread_t tid[27]; //array that stores id
    param_struct* params = (param_struct*)malloc(sizeof(param_struct) * 27);
    int thread_count = 0;

    // validate rows and columns
    for (int i = 0; i < 9; i++) {
        // validate rows
        params[thread_count].starting_row = i;
        params[thread_count].starting_col = 0;
        pthread_create(&tid[thread_count], NULL, validateRow, &(params[thread_count]));
        pthread_join(tid[thread_count++], NULL);

        // validate columns
        params[thread_count].starting_row = 0;
        params[thread_count].starting_col = i;
        pthread_create(&tid[thread_count], NULL, validateCol, &(params[thread_count]));
        pthread_join(tid[thread_count++], NULL);
    }

    // validate squares
    for (int e = 2; e < 9; e += 3) {
        for (int f = 2; f < 9; f += 3) {
            params[thread_count].starting_row = e - 2;
            params[thread_count].starting_col = f - 2;
            pthread_create(&tid[thread_count], NULL, validateSquare, &(params[thread_count]));
            pthread_join(tid[thread_count++], NULL);
        }
    }

    free(params);
    return worker_validation;
}