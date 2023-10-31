#include <stdio.h>
#include <string.h>

#include "regex.h"
#include "matrix.h"
#include "common.h"
#include "controller.h"

void fillEquationsMatrix(char ***equations, int **table1, int rows, int columns)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
            if (table1[i][j] >= 0) {
                int landingState = table1[i][j];
                char *alphabet = get_symbols();

                char symbol[2];
                strncpy(symbol, alphabet+j, 1);

                strcpy(equations[landingState][i], symbol);

                // Print matrix

                printf("Print equations matrix:");
                printf("fila: %d, columna: %d", i, j);
                printf("{\n");
                for(int i = 0; i < rows; i++)
                {
                    printf("[%d]{", i);
                    for(int j = 0; j < rows; j++)
                    {
                        //if(strlen(equations[i][j] ) == 0 ) 
                        if(equations[i][j][0] == '\0')
                        {
                            strcpy(equations[i][j], "-");
                        }
                        printf("%s,", equations[i][j]);
                    }
                    printf("}\n");
                }
                printf("}\n");
            }
		}
	}
}


void fillStartEquationsMatrix(char ***equations, int rows)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < rows; j++)
		{
            strcpy(equations[i][j], "");
		}
	}
}

void createEquationsMatrix(int **table1, int rows, int columns)
{
    char ***equations = (char ***)create3DMatrix(rows, rows, 100, sizeof(char));

    fillStartEquationsMatrix(equations, rows);
    fillEquationsMatrix(equations, table1, rows, columns);
}