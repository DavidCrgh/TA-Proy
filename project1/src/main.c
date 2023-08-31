#include <stdlib.h>
#include <stdio.h>
#include "dfa.h"

int main(int argc, char **argv) 
{
	printf("Hello World\n");
	//testingDFA();
	return 0;
}


void testingDFA()
{
	int size = 9;
	int lista1[] = {3, 1, 2, 0};
	int lista2[] = {0, 1, 2, 0};
	int lista3[] = {3, 1, 3, 0};
	int lista4[] = {3, 3, 3, 3};

	int *tabla[] = {lista1, lista2, lista3, lista4};
	
	int accept[4] = {1, 1, 1, 0};
	
	int *sequence = (int *)malloc(size * sizeof(int));
	
	for(int i = 0; i < size; i++)
	{
		sequence[i] = -1;
	}
	
	int result = dfa_driver(tabla, accept, code, "CTGCTGCTG", 0, sequence);
   	printf("The integer is: %d\n", result);
   	printf("The string goes through the following states:\n");
   	for(int i = 0; i < size; i++)
	{
		printf("%d,", sequence[i]);
	}
	printf("\n");
   	
   	free(sequence);
}
