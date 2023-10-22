#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edge.h"
#include "node.h"

// accepted parameter default value: 0
node* create_node(char *tag, int accepted)
{
	node* new_node = (node*)malloc(sizeof(node));
	new_node->next = NULL;
	new_node->edges = NULL;
	new_node->accepted = accepted;
	strcpy(new_node->tag, tag);
	return new_node;
}

void add_edge(char symbol, node* src, node* dest)
{
	edge* new_edge = create_edge(symbol, dest);
	new_edge->next = src->edges;
	src->edges = new_edge;
}
