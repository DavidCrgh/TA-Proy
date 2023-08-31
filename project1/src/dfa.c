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
	while (*s)
	{
		sequence[i] = k;
		k = table[k][code(*s++)];
		i++;
	}
		
	return (accept[k]);
}

int code(char c)
{
	char *pos = strchr(symbols, c);
	return pos - symbols;
}
