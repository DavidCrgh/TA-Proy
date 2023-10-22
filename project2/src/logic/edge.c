#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "edge.h"

edge* create_edge(char symbol, node* dest)
{
	edge* new_edge = (edge*) malloc(sizeof(edge));
	new_edge->next = NULL;
	new_edge->dest = dest;
	new_edge->symbol = symbol;
	return new_edge;
}
