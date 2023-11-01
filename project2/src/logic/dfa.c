#include <stdio.h>
#include <string.h>

#include "dfa.h"
#include "node.h"
#include "graph.h"
#include "common.h"

int dfa_driver(
	int **table,
	int *accept,
	char *symbols,
	int (*code) (char c, char *symbols),
	char *string,
	int state,
	int *sequence)
{
	char *s;
	int k;
	
	s = string;
	k = state;
	
	int i = 0; 

	while (*s && k != -1)
	{
		sequence[i++] = k;
		k = table[k][code(*s++, symbols)];
	}
	sequence[i] = k;

	return (k == -1 ? 0 : accept[k]);
}

int code(char c, char *symbols)
{
	char *pos = strchr(symbols, c);
	return pos - symbols;
}

graph create_graph(int **table, int *accept, char **tags, char *symbols)
{
	graph new_graph;
	new_graph.nodes = NULL;
	reset_counter(); // Resets counterID for nodes
	
	int total_nodes = num_states;
	int total_symbols = num_symbols;
	
	// Create nodes and add it to graph
	for (int i = 0; i < total_nodes; i++)
	{
		add_node(&new_graph, create_node(tags[i], accept[i]));
	}
	
	// Create edges and add it to graph
	for(int i = 0; i < total_nodes; i++)
	{
		node* src_node = get_node(&new_graph, i);
		for(int j = 0; j < total_symbols; j++)
		{
			node* dest_node = get_node(&new_graph, table[i][j]);
			if(dest_node != NULL || table[i][j] != -1)
				add_edge(symbols[j], src_node, dest_node);	
		}
	}	
	
	return new_graph;
}
