#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "test.h"
#include "../logic/dfa.h"
#include "../logic/matrix.h"
#include "../logic/common.h"

void testingDFA()
{
	srand(time(NULL));
	
	
	symbols = "ACGT";
	char *input = "CGGA";
	
	int rows = num_states;
	int columns = num_symbols;
	printf("Rows and columns: %d %d", rows, columns);
	int len_input = strlen(input);
	
	
	int **table = createMatrix(rows, columns);
	int *sequence = createList(len_input + 1);
	int *accept = createList(rows);
	
	//fill lists and table with random numbers (for testing)
	
	fillListRandom(accept, rows, 0, 1);
	fillMatrixRandom(table, rows, columns, 0, rows - 1);
	
	
	
	int result = dfa_driver(table, accept, code, input, 0, sequence);
   	
   	// Print matrix
   	printf("Print matrix:");
   	printf("{\n");
   	for(int i = 0; i < rows; i++)
	{
		printf("[%d]{", i);
		for(int j = 0; j < columns; j++)
		{
			printf("%d,", table[i][j]);
		}
		printf("}\n");
	}
	printf("}\n");
	
	//Print accept
	printf("Print accept:");
	for(int i = 0; i < rows; i++)
	{
		printf("%d,", accept[i]);
	}
	
	printf("\n");
	
	//Print result
   	printf("Result: %d\n", result);
   	printf("The string goes through the following states:\n");
   	for(int i = 0; i < len_input + 1; i++)
	{
		printf("%d,", sequence[i]);
	}
	
	
	printf("\n");
   	
   	
   	free(accept);
   	free(sequence);
   	freeMatrix(table, rows);
}
