#ifndef CONTROLLER_H
#define CONTROLLER_H


void set_machine_config(int **table, char **state_labels, int *accept, char *symbols, int first_state);
int execute_machine(char *input, int *sequence);

#endif // CONTROLLER_H