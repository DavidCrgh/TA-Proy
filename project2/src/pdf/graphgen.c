#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "graphgen.h"
#include "logic/dfa.h"
#include "logic/node.h"
#include "logic/edge.h"
#include "logic/graph.h"
#include "logic/common.h"

void generate_graph_section(FILE *out, graph pGraph)
{
	fputs("\n\\section{Dibujo DFA}\n", out);
	fputs("\\begin{figure}[htb]\n", out);
	fputs("  \\centering\n", out);
	// GRAPH
	fputs("  \\begin{dot2tex}\n", out);
	fputs("    digraph G {\n", out);
	fputs("      d2tfigpreamble = \"\\tikzstyle{every state}=[very thick]\";\n", out);
	fputs("      edge [lblstyle=\"auto\"];\n", out);
	
	node *actual_node = pGraph.nodes;
	while(actual_node)
	{
		// Add node
		int src_id = actual_node->id;
		int accepted = actual_node->accepted;
		fprintf(out, "      q%d[style=\"state", src_id);
		if(src_id == 1) fputs(",initial", out);
		if(accepted)
			fputs(",accepting, draw=green!50, fill=green!20", out);
		else
			fputs(", draw=red!50, fill=red!20", out);
		fprintf(out, "\",label=\"%s\"];\n",actual_node->tag);
		
		// Add edges
		edge* actual_edge = actual_node->edges;
		while(actual_edge)
		{
			fprintf(out, "      q%d->q%d [label=\"", src_id, actual_edge->dest->id);
			for(int i = 0; i < strlen(actual_edge->symbols); i++)
			{
				fprintf(out, "%c", actual_edge->symbols[i]);
				if(actual_edge->symbols[i + 1] != '\0')
				{
					fputs(", ", out);
				}
			}
			fputs("\"];\n", out);
			actual_edge = actual_edge->next;
		}
		actual_node = actual_node->next;
	}
	fputs("    }\n", out);
	fputs("  \\end{dot2tex}\n", out);
	//END GRAPH
	fputs("  \\caption{Grafo DFA}\n", out);
	fputs("\\end{figure}\n", out);	
}

void build_graph(FILE *out, machine_conf_t *conf) 
{
	graph graph = create_graph(conf->table, conf->accept, conf->labels, conf->symbols);
	fputs("\n\\newpage\n\n", out);
	generate_graph_section(out, graph);
}
