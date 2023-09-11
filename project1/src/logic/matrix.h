#ifndef MATRIX_H
#define MATRIX_H

void fillMatrixRandom(int **matrix, int row, int columns, int min, int max);
void **createMatrix(int rows, int columns, size_t size);
void freeMatrix(int **table, int rows);

#endif
