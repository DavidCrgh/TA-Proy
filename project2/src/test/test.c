#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "test.h"
#include "../logic/dfa.h"
#include "../logic/list.h"
#include "../logic/matrix.h"
#include "../logic/common.h"

void fillMatrixRandom(int **matrix, int row, int columns, int min, int max)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			matrix[i][j] = (rand() % (max - min + 1)) + min;
		}
	}
}

void fillListRandom(int *list, int total, int min, int max)
{
	for(int i = 0; i < total; i++)
	{
		list[i] = (rand() % (max - min + 1)) + min;
	}
}

void testingDFA()
{
	srand(time(NULL));
	
	
	char *symbols = "ACGT";
	char *input = "CGGA";
	
	int rows = 4;
	int columns = 4;
	printf("Rows and columns: %d %d", rows, columns);
	int len_input = strlen(input);
	
	
	int **table = (int **)createMatrix(rows, columns, sizeof(int));
	int *sequence = (int*)createList(len_input + 1, sizeof(int));
	int *accept = (int *)createList(rows, sizeof(int));
	
	//fill lists and table with random numbers (for testing)
	
	fillListRandom(accept, rows, 0, 1);
	fillMatrixRandom(table, rows, columns, -1, rows - 1);
	
	int result = dfa_driver(table, accept, symbols, code, input, 0, sequence);
	//int result = execute_machine(input, sequence);
   	
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
   	freeMatrix((void**)table, rows);
}


void print_machine_config(machine_conf_t *conf) {

	printf("////////////////////////////////////////////////\n");
	printf("MACHINE CONFIGURATION\n\n");

	printf("num_states: %d\tnum_symbols: %d\n", conf->num_states, conf->num_symbols);
	printf("first_state: %d\tsymbols: %s\n\n\n", conf->first_state, conf->symbols);

	printf("STATES: \n\n");
	printf("Index\tTag\tAcceptance?\n");

	for(int i = 0; i < conf->num_states; i++) {

		printf(
			"%d\t%s\t%d\n", 
			i + 1,
			conf->labels[i],
			conf->accept[i]);

	}

	printf("\n\n\n");

	printf("TRANSITION TABLE: \n\n");

	for (int i = 0; i < conf->num_states; i++) {

		for (int j = 0; j < conf->num_symbols; j++) {
			
			printf("%d (%d,%c)\t", conf->table[i][j], i + 1, conf->symbols[j]);

		}

		printf("\n\n");

	}

	printf("////////////////////////////////////////////////\n\n");

}
