#ifndef MATRIX_H
#define MATRIX_H

void errorMessage(char *message);
void fillListRandom(int *list, int total, int min, int max);
void fillMatrixRandom(int **matrix, int row, int columns, int min, int max);
int *createList(int total);
void fillList(int *list, int total);
int **createMatrix(int rows, int columns);
void freeMatrix(int **table, int rows);

#endif
