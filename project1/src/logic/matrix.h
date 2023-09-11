#ifndef MATRIX_H
#define MATRIX_H

void fillMatrixRandom(int **matrix, int row, int columns, int min, int max);
int **createMatrix(int rows, int columns);
void freeMatrix(int **table, int rows);

#endif
