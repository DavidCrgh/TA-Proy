#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edge.h"
#include "node.h"

int counterID = 1;

// accepted parameter default value: 0
node* create_node(char *tag, int accepted)
{
	node* new_node = (node*)malloc(sizeof(node));
	new_node->id = counterID++;
	new_node->next = NULL;
	new_node->edges = NULL;
	new_node->accepted = accepted;
	if (tag != NULL) strcpy(new_node->tag, tag);
	
	return new_node;
}

void add_edge(char symbol, node* src, node* dest)
{
	int dest_id = dest->id;
	edge* actual_edge = src->edges;
	while(actual_edge)
	{
		if(actual_edge->dest->id == dest_id)
		{
			int length = strlen(actual_edge->symbols);
			actual_edge->symbols[length] = symbol;
			actual_edge->symbols[length + 1] = '\0';
			return;
		}
		actual_edge = actual_edge->next;
	}
	edge* new_edge = create_edge(symbol, dest);
	new_edge->next = src->edges;
	src->edges = new_edge;
}

void reset_counter() {
	counterID = 1;
}
