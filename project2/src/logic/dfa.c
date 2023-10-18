#include <stdio.h>
#include <string.h>
#include "dfa.h"


int dfa_driver(
	int **table,
	int *accept,
	char *symbols,
	int (*code) (char c, char *symbols),
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
		k = table[k][code(*s++, symbols)];
	}
	sequence[i] = k;

	return (k == -1 ? 0 : accept[k]);
}

int code(char c, char *symbols)
{
	char *pos = strchr(symbols, c);
	return pos - symbols;
}
