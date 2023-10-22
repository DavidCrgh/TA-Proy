#ifndef NODE_H
#define NODE_H

typedef struct node node;
typedef struct edge edge;

struct node
{
	char tag[20];
	edge* edges;
	node* next;
	int accepted;
};

node* create_node(char *tag, int accepted);
void add_edge(char symbol, node* src, node *dest);

#endif
