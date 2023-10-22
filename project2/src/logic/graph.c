#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "edge.h"
#include "graph.h"

void add_node(graph* pGraph, node* pNode)
{
	if(pGraph->nodes == NULL)
	{
		pGraph->nodes = pNode;
	}
	else
	{
		node* ptr_node;
		ptr_node = pGraph->nodes;
		while(ptr_node->next)
		{
			ptr_node = ptr_node->next;
		}
		ptr_node->next = pNode;
	}
}

node* get_node(graph* pGraph, int pos)
{
	node* actual_node = pGraph->nodes;
	int actual_pos = 0;
	
	while(actual_node != NULL)
	{
		if(pos == actual_pos)
			return actual_node;
			
		actual_pos++;
		actual_node = actual_node->next;
	}
	
	return NULL;
}

void layout(graph* pGraph)
{
	node* actual_node = get_node(pGraph, 0);
	while(actual_node != NULL)
	{
		printf("Node %s:\n", actual_node->tag);
		edge* actual_edge = actual_node->edges;
		while(actual_edge != NULL)
		{
			printf("  --(%c)--> %s\n", actual_edge->symbol, actual_edge->dest->tag);
			actual_edge = actual_edge->next;
		}
		actual_node = actual_node->next;
	}
}
