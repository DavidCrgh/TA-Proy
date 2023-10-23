#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "edge.h"
#include "graph.h"

void generate_dfa_latex(graph pGraph)
{
	const char* file_name = "report.tex";
	
	FILE* file = fopen(file_name, "w");
	
	if(file == NULL)
	{
		printf("Can't open file.\n");
		return;
	}
	
	fprintf(file, "\\documentclass{article}\n");
	fprintf(file, "\\usepackage{tikz}\n");
	fprintf(file, "\\usetikzlibrary{arrows,shapes,automata}\n");
	fprintf(file, "\\usepackage{dot2texi}\n\n\n");
	fprintf(file, "\\begin{document}\n");
	fprintf(file, "\\section{Dibujo DFA}\n");
	// Graph code
	fprintf(file, "\\begin{tikzpicture}[>=latex', scale=0.8]\n");
	fprintf(file, "\\begin{dot2tex}[circo]\n");
	fprintf(file, "digraph G {\n");
	fprintf(file, "  node [style=\"state\"];\n");
	fprintf(file, "  edge [lblstyle=\"auto\",topath=\"bend left\"];\n");
	
	node* actual_node = pGraph.nodes;
	while(actual_node)
	{
		//Add node
		int src_id = actual_node->id;
		int accepted = actual_node->accepted;
		fprintf(file, "  q%d[style=\"state", src_id);
		if(src_id == 1) fprintf(file, ",initial");
		if(accepted) fprintf(file, ",accepting");
		fprintf(file, "\",label=\"%s\"];\n",actual_node->tag);
		
		//Add edges
		edge* actual_edge = actual_node->edges;
		while(actual_edge)
		{
			fprintf(file, "  q%d->q%d [label=\"", src_id, actual_edge->dest->id);
			for(int i = 0; i < strlen(actual_edge->symbols); i++)
			{
				fprintf(file, "%c", actual_edge->symbols[i]);
				if(actual_edge->symbols[i + 1] != '\0')
				{
					fprintf(file, ", ");
				}
			}
			
			if(src_id == actual_edge->dest->id) fprintf(file, "\",topath=\"loop above");
			fprintf(file, "\"];\n");
			actual_edge = actual_edge->next;
		}
		actual_node = actual_node->next;
	}
	//fprintf(file, "  A[style=\"state,initial,accepting\"];\n");
	//fprintf(file, "  B[style=\"state\"];\n");
	//fprintf(file, "  C[style=\"state,accepting\"];\n");
	// Iterate graph
	// End iterate graph
	//fprintf(file, "	A->B [label=\"0\"];\n");
	//fprintf(file, "	B->C [label=\"1\"];\n");
	//fprintf(file, "	C->A [label=\"0\"];\n");
	//fprintf(file, "	C->D [label=\"1\"];\n");
	//fprintf(file, "	D->D [label=\"0\"];\n");
	fprintf(file, "}\n");
	fprintf(file, "\\end{dot2tex}\n");
	fprintf(file, "\\end{tikzpicture}\n");
	// End graph code
	fprintf(file, "\\end{document}");
	
	fclose(file);
}
