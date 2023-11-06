#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "regex.h"
#include "matrix.h"
#include "list.h"
#include "common.h"
#include "controller.h"

#define EPSILON "\u03B5"
#define MAX_STRING_LEN 1024

FILE *out;
char **tags;

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

                if(strcmp(equations[landingState][i], "") == 0)
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
                strcpy(equations[i][j], "\\textepsilon ");
            } else {
                strcpy(equations[i][j], "");
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
                char *tempString = (char*)createList(n + 2, sizeof(char));
                sprintf(tempString, "(%s)", equations[i][j]);
                strcpy(equations[i][j], tempString);
            }

            
		}
	}
}

void save_regex_change(int state, char **equation)
{
    int flag = 0; // Flag para saber si debemos imprimir un +
    fprintf(out, "\t\\item %s = ", tags[state]);
    for(int k = 0; k <= num_states; k++)
    {
        if(strcmp(equation[k], "") != 0)
        {
            if(flag)
            {
                fputs(" + ", out);
            }
            if(k != num_states)
            {
                fprintf(out, "%s%s", tags[k], equation[k]);    
            }
            else
            {
                fprintf(out, "%s", equation[k]);   
            }
            flag = 1;
        }
    }
    fputs(" \n", out);
}

char *generateRegex(int state, char ***equations, int *processedStates, int num_states)
{   
    int equationsChanged = 1;
    if(out != NULL)
    {
        save_regex_change(state, equations[state]);
    }

    while (equationsChanged)
    {
        equationsChanged = 0;

        for(int i = 0; i < num_states; i++)
        {
            if (i != state)
            {
                if(strcmp(equations[state][i], "") != 0) 
                {
                    if(strcmp(equations[i][i], "") != 0) 
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
                            if(strcmp(equations[i][j], "") != 0)
                            {
                                if(strcmp(equations[state][j], "") != 0)
                                {
                                    char *tempString = (char *)createList(MAX_STRING_LEN, sizeof(char));
                                    sprintf(tempString, "(%s + %s%s)", equations[state][j], equations[i][j], equations[state][i]);
                                    strcpy(equations[state][j], tempString);
                                } 
                                else 
                                {
                                    sprintf(equations[state][j], "%s%s", equations[i][j], equations[state][i]);
                                }
                            }
                        }

                        strcpy(equations[state][i], "");  

                        if(out != NULL)
                        {
                            save_regex_change(state, equations[state]);
                        }
                        equationsChanged = 1;
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

    char *result = (char *)createList(MAX_STRING_LEN, sizeof(char));
    strcpy(result, "");

    // Arden
    for(int j = 0; j <= num_states; j++)
    {
        if((j != state) && (strcmp(equations[state][j], "") != 0) && (strcmp(equations[state][state], "") != 0))
        {
            char *tempString = (char *)createList(MAX_STRING_LEN, sizeof(char));
            sprintf(tempString, "%s(%s)*", equations[state][j], equations[state][state]);
            strcpy(equations[state][j], tempString);
        }
    }
    equations[state][state] = "";
    for(int j = 0; j <= num_states; j++)
    {
        strcat(result, equations[state][j]);
    }
    if(out != NULL)
    {
        save_regex_change(state, equations[state]);
    }
    return result;
}

int getAmountAcceptanceStates(int *acceptanceStates, int num_states)
{
    int count = 0;
    for(int i = 0; i < num_states; i++){
        if (acceptanceStates[i] == 1)
        {
            count++;
        }
    }
    return count;
}

char *getRegexOut(FILE * out_regex, char **labels, int **table1, int *accept, int num_states, int num_symbols)
{
    out = out_regex;
    tags = labels;
    return getRegex(table1, accept, num_states, num_symbols);
}

char *getRegex(int **table1, int *accept, int num_states, int num_symbols)
{
    char ***equations = (char ***)create3DMatrix(num_states, num_states + 1, MAX_STRING_LEN, sizeof(char));

    fillStartEquationsMatrix(equations, num_states, num_states + 1);
    fillEquationsMatrix(equations, table1, num_states, num_symbols);
    addParentheses(equations, num_states);

    int *processedStates = (int *)createList(num_states, sizeof(int));

    printf("Print equations matrix i=%d", 0);
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

    for(int k = 0; k < num_states; k++)
    {
        processedStates[k] = 0;
    }

    int *acceptanceStates = accept;

    if(out != NULL)
    {
        fputs("El sistema de ecuaciones inicial es: \n", out);
        fputs("\\begin{itemize}\n", out);
        for(int i = 0; i < num_states; i++)
        {
            save_regex_change(i, equations[i]);
        }
        fputs("\\end{itemize}\n", out);
    }
    else
    {
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
    }

    char *regex = (char *)createList(MAX_STRING_LEN, sizeof(char));
    memset(regex,0,strlen(regex));
    int remainingAcceptanceStates = getAmountAcceptanceStates(acceptanceStates, num_states);

    
    if(out != NULL)
    {
        fputs("\nLos pasos a realizar son: \n", out);
        fputs("\\begin{itemize}\n", out);
    }
    for(int i=0; i < num_states; i++) 
    {
        if(acceptanceStates[i] == 1 && processedStates[i] == 0)
        {
            processedStates[i] = 1;
            
            char *tempString = (char *)createList(MAX_STRING_LEN, sizeof(char));
            char *tempString2 = (char *)createList(MAX_STRING_LEN, sizeof(char));
            memset(tempString,0,strlen(tempString));
            memset(tempString2,0,strlen(tempString2));

            tempString2 = generateRegex(i, equations, processedStates, num_states);
            if(remainingAcceptanceStates > 1) 
            {
                sprintf(tempString, "%s + ", tempString2);
            } 
            else
            {
                sprintf(tempString, "%s", tempString2);
            }
            strcat(regex, tempString);
            remainingAcceptanceStates--;
            free(tempString);
            free(tempString2);
        }
    }
    if(out != NULL)
    {
        fputs("\\end{itemize}\n", out);
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

    freeMatrix((void**) equations, num_states);

    return regex;
}
