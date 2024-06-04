#ifndef MATRIX_H
#define MATRIX_H

void **createMatrix(int rows, int columns, size_t size);
void ***create3DMatrix(int x, int y, int z, size_t size);
void freeMatrix(void **table, int rows);

#endif
