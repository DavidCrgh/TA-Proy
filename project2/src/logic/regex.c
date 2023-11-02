#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "regex.h"
#include "matrix.h"
#include "list.h"
#include "common.h"
#include "controller.h"

#define EPSILON "\u03B5"

void fillEquationsMatrix(char ***equations, int **table1, int num_states, int columns)
{
	for (int i = 0; i < num_states; i++)
	{
		for (int j = 0; j < columns; j++)
		{
            if (table1[i][j] >= 0) {
                int landingState = table1[i][j];
                char *alphabet = get_symbols();

                char symbol[2];
                strncpy(symbol, alphabet+j, 1);

                if(strcmp(equations[landingState][i], " ") == 0)
                {
                    strcpy(equations[landingState][i], symbol);
                } else {
                    strcat(equations[landingState][i], "+");
                    strcat(equations[landingState][i], symbol);
                }

                
            }
		}
	}
}


void fillStartEquationsMatrix(char ***equations, int num_states, int columns)
{
	for (int i = 0; i < num_states; i++)
	{
		for (int j = 0; j < columns; j++)
		{
            if(i == 0 && (j == columns-1)) {
                strcpy(equations[i][j], EPSILON);
            } else {
                strcpy(equations[i][j], " ");
            }
		}
	}
}

void addParentheses(char ***equations, int num_states){
    for (int i = 0; i < num_states; i++)
	{
		for (int j = 0; j < num_states; j++)
		{
            size_t n = strlen(equations[i][j]);
            if (n > 2) {                
                char *temp = (char*)createList(n + 2, sizeof(char));
                strcpy(temp, "(");
                strcat(equations[i][j], ")");
                strcat(temp, equations[i][j]);
                strcpy(equations[i][j], temp);
            }
            
		}
	}
}

void arden(){
    
}

void generateRegex(int state, char ***equations, int *processedStates, int num_states)
{
    for(int i; i > num_states + 1; i++) {
        if (i != state){
            if(strcmp(equations[state][i], " ") != 0) {
                if(strcmp(equations[i][i], " ") != 0) {
                    if(strcmp(equations[state][i], " ") != 0){
                        if(processedStates[i] == 0) {

                        } else {
                            processedStates[i] = 1;
                            generateRegex(i, equations, processedStates, num_states);
                            for(int j; j > num_states; j++) {
                                if(strcmp(equations[i][j], " ") != 0){
                                    if(strcmp(equations[state][j], " ") != 0){
                                        // matriz[estado'][k] = "(" + matriz[estado'][k] + "+" + matriz[j][k] + matriz[estado'][j] + ")"
                                        // matriz[estado'][k] = matriz[j][k] + matriz[estado'][j]
                                    }
                                } else {
                                    equations[state][j] = " ";
                                }
                            }
                        }
                    }

                }
            }
        }

        arden();
    }

}

void createEquationsMatrix(int **table1, int *accept, int num_states, int num_symbols)
{
    char ***equations = (char ***)create3DMatrix(num_states, num_states + 1, num_states * num_symbols, sizeof(char));

    fillStartEquationsMatrix(equations, num_states, num_states + 1);
    fillEquationsMatrix(equations, table1, num_states, num_symbols);
    addParentheses(equations, num_states);

    int *processedStates = (int *)createList(num_states, sizeof(int));
    int *acceptanceStates = accept;

    /*
    for(int k = 0; k < num_states; k++)
    {
        printf("%d, ", acceptanceStates[k]);
    }

    // Print matrix

    printf("Print equations matrix:");
    printf("{\n");
    for(int i = 0; i < num_states; i++)
    {
        printf("[%d]{", i);
        for(int j = 0; j < num_states + 1; j++)
        {
            printf("%s,", equations[i][j]);
        }
        printf("}\n");
    }
    printf("}\n");
    */

    for(int i=0; i < num_states; i++) {
        if(acceptanceStates[i] == 1 && processedStates[i] == 0){
            processedStates[i] = 1;
            generateRegex(i, equations, processedStates, num_states);
        }
    }

}