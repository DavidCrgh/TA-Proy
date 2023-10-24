#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "common.h"

void free_config();
void set_machine_config(int **table, char **state_labels, int *accept, char *symbols, int first_state);
int execute_machine(char *input, int *sequence);
char **get_state_labels();
char *get_symbols();
machine_conf_t *get_conf();

#endif // CONTROLLER_H
