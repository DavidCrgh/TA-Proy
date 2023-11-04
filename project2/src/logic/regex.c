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

void arden(int state, char ***equations, int num_states)
{
    //TODO manejar caso donde hay un epsilon
    sprintf(equations[state][num_states], "%s*", equations[state][state]);
    sprintf(equations[state][state], " ");
}

char *generateRegex(int state, char ***equations, int *processedStates, int num_states)
{   
    int equationsChanged = 1;

    while (equationsChanged)
    {
        equationsChanged = 0;

        for(int i = 0; i < num_states; i++)
        {
            if (i != state)
            {
                if(strcmp(equations[state][i], " ") != 0) 
                {
                    if(strcmp(equations[i][i], " ") != 0) 
                    {
                        if(processedStates[i] == 0) 
                        {
                            processedStates[i] = 1;
                            generateRegex(i, equations, processedStates, num_states);
                            processedStates[i] = 0;
                            equationsChanged = 1;
                        } 
                    }
                    else 
                    {
                        for(int j = 0; j <= num_states; j++) 
                        {
                            if(strcmp(equations[i][j], " ") != 0)
                            {
                                if(strcmp(equations[state][j], " ") != 0)
                                {
                                    char *tempString = (char *)createList(num_states*num_states, sizeof(char));
                                    sprintf(tempString, "(%s+%s%s)", equations[state][j], equations[i][j], equations[state][i]);
                                    strcpy(equations[state][j], tempString);
                                } 
                                else 
                                {
                                    sprintf(equations[state][j], "%s%s", equations[i][j], equations[state][i]);
                                }
                            }
                            equations[state][i] = " ";
                            equationsChanged = 1;
                        }  
                    }
                }
            }
            printf("Print equations matrix i=%d", i);
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
        }
    }

    char *result = (char *)createList(num_states*num_states, sizeof(char));
    for(int j = 0; j <= num_states; j++)
    {
        if((j != state) && (strcmp(equations[state][j], " ") != 0))
        {
            char *tempString = (char *)createList(num_states*num_states, sizeof(char));
            sprintf(tempString, "%s(%s)*", equations[state][j], equations[state][state]);
            strcpy(equations[state][j], tempString);
            strcat(result, equations[state][j]);
        }
    }

    equations[state][state] = "";
    return result;
}

void createEquationsMatrix(int **table1, int *accept, int num_states, int num_symbols)
{
    char ***equations = (char ***)create3DMatrix(num_states, num_states + 1, num_states * num_states, sizeof(char));

    fillStartEquationsMatrix(equations, num_states, num_states + 1);
    fillEquationsMatrix(equations, table1, num_states, num_symbols);
    addParentheses(equations, num_states);

    int *processedStates = (int *)createList(num_states, sizeof(int));

    for(int k = 0; k < num_states; k++)
    {
        processedStates[k] = 0;
    }

    int *acceptanceStates = accept;

    printf("Print equations matrix before regex generator");
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

    for(int i=0; i < num_states; i++) 
    {
        if(acceptanceStates[i] == 1 && processedStates[i] == 0)
        {
            processedStates[i] = 1;
            printf("REGEX = %s\n", generateRegex(i, equations, processedStates, num_states));
            //TODO encontrar el lugar correcto para aplicar Arden
            //arden(i, equations, num_states);
        }
    }

    printf("Print equations matrix after regex generator");
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
}