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
		errorMessage("Error to assign memory");
	}
	
	for(int i = 0; i < rows; i++)
	{
		matrix[i] = createList(columns, size);
		if(matrix[i] == NULL)
		{
			errorMessage("Error to assign memory");
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
