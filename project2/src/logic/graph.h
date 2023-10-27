#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

typedef struct graph graph;

struct graph
{
	node* nodes;
};

void add_node(graph* pGraph, node* pNode);
node* get_node(graph* pGraph, int pos);
void layout(graph* pGraph);

#endif
