#ifndef DFA_H
#define DFA_H

int dfa_driver(int **table, int *accept, char *symbols, int (*code) (char c, char *symbols), char *string, int state, int *sequence);
int code (char c, char *symbols);

#endif
