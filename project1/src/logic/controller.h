#ifndef CONTROLLER_H
#define CONTROLLER_H


void set_machine_config(int **table, char **state_labels, int *accept, char *symbols, int first_state);
int execute_machine(char *input, int *sequence);
char **get_state_labels();

#endif // CONTROLLER_H