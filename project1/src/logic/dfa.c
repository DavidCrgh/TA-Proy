#include <stdio.h>
#include <string.h>
#include "dfa.h"


int dfa_driver(
	int **table,
	int *accept,
	int (*code) (char c),
	char *string,
	int state,
	int *sequence)
{
	char *s;
	int k;
	
	s = string;
	k = state;
	
	int i = 0; 

	while (*s && k != -1)
	{
		sequence[i++] = k;
		k = table[k][code(*s++)];
	}
	sequence[i] = k;
	return (accept[k-1]);
}

int code(char c)
{
	char *pos = strchr(symbols, c);
	return pos - symbols;
}
