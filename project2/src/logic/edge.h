#ifndef EDGE_H
#define EDGE_H

#include "node.h"

typedef struct edge edge;

struct edge
{
	char symbols[100];
	node* dest;
	edge* next;
};

edge* create_edge(char symbol, node* dest);

#endif
