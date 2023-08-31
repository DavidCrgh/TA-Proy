#ifndef DFA_H
#define DFA_H


int dfa_driver(int **table, int *accept, int (*code) (char c), char *string, int state, int *sequence);
int code (char c);

#endif
