#ifndef REGEX_H
#define REGEX_H

char *getRegexOut(FILE * out_regex, char ** labels, int **table1, int *accept, int num_states, int num_symbols);
char *getRegex(int **table1, int *accept, int rows, int columns);

#endif
