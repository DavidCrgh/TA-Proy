#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

int num_states = 0;
int num_symbols = 0;

void errorMessage(char *message)
{
	perror(message);
	exit(1);
}

char *get_char(char c)
{
	static char result[2];
	result[0] = c;
	result[1] = '\0';
	return result;
}


