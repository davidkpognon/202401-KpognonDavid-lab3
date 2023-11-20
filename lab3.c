#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab3.h"
#define SIZE 9
#define MACRO_NAME value


int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** board = NULL;
    //need to create a integer array board
    int sudoku[9][9] ;
    fp = fopen("invalid_board.txt", "r");
    
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (fscanf(fp, "%d", &sudoku[i][j]) != 1) {
                fprintf(stderr, "Error reading from file");
                fclose(fp);
                return 1; // Exit with an error code
            }
        }
    }
    board = &sudoku;
    // Store the content of the file
    // fgets(myString, 100, fp);

    return board;
}

void* validateColumn(void* arg) {
    int col = *(int*)arg;
    int digits[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[i][col];
        if (digits[num - 1] == 1 || num < 1 || num > SIZE) {
            printf("Column %d is invalid.\n", col);
            pthread_exit(NULL);
        }
        digits[num - 1] = 1;
    }

    printf("Column %d is valid.\n", col);
    pthread_exit(NULL);
}

pthread_t column_threads[SIZE];
    int col_args[SIZE];

    // Create threads to validate each column
    for (int i = 0; i < SIZE; i++) {
        col_args[i] = i;
        if (pthread_create(&column_threads[i], NULL, validateColumn, &col_args[i]) != 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

    // Wait for column threads to complete
    for (int i = 0; i < SIZE; i++) {
        if (pthread_join(column_threads[i], NULL) != 0) {
            perror("Thread join failed");
            return 1;
        }
    }

    //Line 68-128 are questionable not certain

    int sudoku[SIZE][SIZE];

// Function to check if an array of 9 integers contains all digits from 1 to 9
int isValid(int arr[SIZE]) {
    int seen[SIZE] = {0};

    for (int i = 0; i < SIZE; i++) {
        if (arr[i] < 1 || arr[i] > SIZE || seen[arr[i] - 1] == 1) {
            return 0;
        }
        seen[arr[i] - 1] = 1;
    }

    return 1;
}

// Function to validate a Sudoku board by column
int validateColumns() {
    for (int col = 0; col < SIZE; col++) {
        int column[SIZE];
        for (int row = 0; row < SIZE; row++) {
            column[row] = sudoku[row][col];
        }
        if (!isValid(column)) {
            return 0;
        }
    }
    return 1;
}

// Function to validate a Sudoku board by row
int validateRows() {
    for (int row = 0; row < SIZE; row++) {
        int rowArr[SIZE];
        for (int col = 0; col < SIZE; col++) {
            rowArr[col] = sudoku[row][col];
        }
        if (!isValid(rowArr)) {
            return 0;
        }
    }
    return 1;
}

// Function to validate a Sudoku board by 3x3 subgrids
int validateSubgrids() {
    for (int startRow = 0; startRow < SIZE; startRow += 3) {
        for (int startCol = 0; startCol < SIZE; startCol += 3) {
            int subgrid[SIZE];
            int index = 0;
            for (int row = startRow; row < startRow + 3; row++) {
                for (int col = startCol; col < startCol + 3; col++) {
                    subgrid[index] = sudoku[row][col];
                    index++;
                }
            }
            if (!isValid(subgrid)) {
                return 0;
            }
        }
    }
    return 1;
}

int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;

    //validate the board here
    int sudoku[SIZE][SIZE];

    if (validateColumns() && validateRows() && validateSubgrids()) {
        return 1; //returns 1 for true if the rows, columns and subgrids are valid
    } else {
        return 0; //returns this if the board is invalid
    }
    
}