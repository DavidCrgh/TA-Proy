#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dfa.h"
#include "matrix.h"

char *symbols;

void testingDFA()
{
	srand(time(NULL));
	
	char *input = "CTGA";
	int rows = 3;
	int columns = 3;
	int len_input = strlen(input);
	symbols = "ACGT";
	
	int **table = createMatrix(rows, columns);
	int *sequence = createList(len_input);
	int *accept = createList(rows);
	
	//fill lists and table with random numbers (for testing)
	
	fillListRandom(accept, rows, 0, 1);
	fillMatrix(table, rows, columns, 0, rows - 1);
	
	
	
	int result = dfa_driver(table, accept, code, input, 0, sequence);
   	printf("Result: %d\n", result);
   	printf("The string goes through the following states:\n");
   	for(int i = 0; i < len_input; i++)
	{
		printf("%d,", sequence[i]);
	}
	printf("\n");
   	
   	
   	free(accept);
   	free(sequence);
   	freeMatrix(table, rows);
}

int main(int argc, char **argv) 
{
	printf("Hello World\n");
	testingDFA();
	return 0;
}


