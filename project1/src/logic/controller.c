#include <stdlib.h>

#include "controller.h"
#include "common.h"
#include "matrix.h"
#include "logic/dfa.h"


static machine_conf_t *config = NULL;

///////////// PRIVATE FUNCTIONS ///////////////////////////////////////////////

void free_config() {

    for (int i = 0; i < config->num_states; i++) {

        free(config->table[i]);
        free(config->labels[i]);

    }

    free(config->table);
    free(config->labels);
    free(config->symbols);
    free(config->accept);

    free(config);
    config = NULL;

}


void init_config() {

    if (config != NULL) {
        free_config();
    }

    config = (machine_conf_t*) malloc(sizeof(machine_conf_t));
    config->num_states = num_states;
    config->num_symbols = num_symbols;

}

///////////// PUBLIC FUNCTIONS ///////////////////////////////////////////////

void set_machine_config(int **table, char **state_labels, int *accept, char *symbols, int first_state) {

    init_config();

    config->table = table;
    config->labels = state_labels;
    config->accept = accept;
    config->symbols = symbols;
    config->first_state = first_state;
}

int execute_machine(char *input, int *sequence) {

    return dfa_driver(
        config->table,
        config->accept,
        code,
        input,
        config->first_state,
        sequence
    );
}

char **get_state_labels() {
    return config->labels;
}