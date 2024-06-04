#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "logic/list.h"
#include "logic/common.h"
#include "logic/regex.h"

char* generate_regex(FILE *out_regex, machine_conf_t *conf)
{
	// Get regular expression algorithm
	return getRegexOut(out_regex, conf->labels, conf->table, conf->accept, conf->num_states, conf->num_symbols);
}

void build_regex(FILE *out, FILE *out_regex, machine_conf_t *conf)
{
	// Call regex mechanism
	// Pass out_regex parameter in case we want to generate step by step
	char *result = generate_regex(out_regex, conf);
	fclose(out_regex);
	fputs("\n\n\\section{Expresion Regular}\n\n", out);
	fputs("La expresión regular asociada al autómata ingresado es:\n", out);
	fputs("\\begin{center}\n", out);
	fprintf(out, "\t%s\n", result);
	fputs("\\end{center}\n\n", out);
}

//void generate_regex(FILE *out_regex, machine_conf_t *conf)
//{
//	fputs("El sistema de ecuaciones inicial es:\n", out_regex);
//	fputs("\\begin{align*}\n", out_regex);
//	//3D Matrix created previously
//	char ***matrix;
//	print_system_equations(out_regex, matrix, conf->labels, conf->num_states);
//}

//void print_system_equations(FILE *out_regex, char ***equation, char **tags, int total_states)
//{
//	for(int i = 0; i < total_states; i++)
//	{
//		int flag_print_add = 0;
//		printf("El sistema de ecuaciones inicial es:\n");
//		printf("\t%s &= ", tags[i]);
//		for(int j = 0; j <= total_states; j++)
//		{
//			if(equation[i][j] != "")
//			{
//				if(flag_print_add)
//				{
//					printf(" + ");
//				}
//				printf("%s%s", tags[i], equation[i][j]);
//				flag_print_add = 1;
//				
//			}
//		}
//		printf(" \\\\\n");
//	}
//}


