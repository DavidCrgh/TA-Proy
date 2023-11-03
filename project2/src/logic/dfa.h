#ifndef DFA_H
#define DFA_H

#include "graph.h"

int dfa_driver(int **table, int *accept, char *symbols, int (*code) (char c, char *symbols), char *string, int state, int *sequence);
int code (char c, char *symbols);
graph create_graph(int **table, int *accept, char **tags, char *symbols);
void add_sink(graph *g, int **table, char *symbols);

#endif
