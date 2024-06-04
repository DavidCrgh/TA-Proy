#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "list.h"
#include "common.h"

// Method that returns a generic matrix, based on rows, columns and an specific size
// To use it is necessary to apply a cast of the type that you want to use.
// Example:
// 			int **matrix = (int **)createMatrix(rows, columns, size);
void **createMatrix(int rows, int columns, size_t size)
{
	void **matrix;
	matrix = malloc(rows * sizeof(void *));
	if(matrix == NULL)
	{
		errorMessage("Error to assign memory3");
	}
	
	for(int i = 0; i < rows; i++)
	{
		matrix[i] = createList(columns, size);
		if(matrix[i] == NULL)
		{
			errorMessage("Error to assign memory4");
		}
	}
	
	return matrix;
}

/*
void ***createMatrix3D(int x, int y, int z, size_t size)
{
	void ***matrix;
	matrix = malloc(rows * columns * sizeof(void *));
	if(matrix == NULL)
	{
		errorMessage("Error to assign memory1");
	}
	
	for(int i = 0; i < rows; i++)
	{
		matrix[i] = createMatrix(rows, columns, size);

		if(matrix[i] == NULL)
		{
			errorMessage("Error to assign memory2");
		}
	}
	
	return matrix;
}*/

void ***create3DMatrix(int x, int y, int z, size_t size)
{
    void ***matrix = (void ***)malloc(x * sizeof(void **));
    for (int i = 0; i < x; i++)
	{
        matrix[i] = (void **)malloc(y * sizeof(void *));
        for (int j = 0; j < y; j++)
		{
            matrix[i][j] = malloc(z * size);
        }
    }
    return matrix;
}

void freeMatrix(void **table, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		free(table[i]);
	}
	free(table);
}
