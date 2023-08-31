#include <stdio.h>
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
	if (c == 'A') return 0;
	if (c == 'T') return 1;
	if (c == 'C') return 2;
	if (c == 'G') return 3;
	return -1;
}
