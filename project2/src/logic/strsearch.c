#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "strsearch.h"
#include "bqueue.h"
#include "common.h"
#include "matrix.h"
#include "graph.h"
#include "node.h"
#include "edge.h"

#define QUEUE_MAX 10000
#define STRLEN_MAX 30
#define NUM_STR 5 // How many strings to generate


static bool qflush_mode = false;

// Obtains the complement of the automata's accepting states.
int *flip_accept_states(int *accept) {
    int *new_accept = (int*) malloc(sizeof(int) * num_states);

    for (int i = 0; i < num_states; i++) {

        new_accept[i] = abs(accept[i] - 1);
    }

    return new_accept;
}


void add_transitions(b_queue_t *q, node *state, char *str) {

    edge *current = state->edges;
    char symbol;

    int symbol_idx = 0;

    char *new_string = NULL;
    int next_state = -1;

    while (current != NULL) {

        symbol = current->symbols[symbol_idx];

        while (symbol != '\0') {

            // Append symbol to the existing string
            new_string = (char *) malloc(sizeof(char) * STRLEN_MAX); // This malloc is paired with free(current_str) in the get_strings function
            strcpy(new_string, str);
            strncat(new_string, &symbol, 1);

            next_state = current->dest->id - 1;

            
            // Add (currentString + nextSymbol, nextState) to the queue
            enqueue(q, new_string, next_state);
            

            // Flush mode is triggered when the queue fills up, this prevents any further writes
            if (q->count == QUEUE_MAX) qflush_mode = true;

            symbol_idx++;
            symbol = current->symbols[symbol_idx];
        }

        current = current->next;
        symbol_idx = 0;
    }
}

char **get_strings(graph *g, machine_conf_t *conf, bool complement) {

    // Some setup required to run the string search
    b_queue_t *queue = init_bqueue(QUEUE_MAX);

    int current_state = -1;
    node *current_node = NULL;

    char *current_str = malloc(sizeof(char) * STRLEN_MAX);
    current_str[0] = '\0'; // First string is epsilon ""

    char **out_strings = (char **) createMatrix(NUM_STR, STRLEN_MAX, sizeof(char*));
    int found = 0;

    int *accepting = complement ? flip_accept_states(conf->accept) : conf->accept;


    // Algorithm starts
    // Add empty string and initial state to queue
    enqueue(queue, current_str, 0); 
    print_bqueue(queue);


    // Iterate through the queue
    while (queue->count > 0) {

        current_str = dequeue(queue, &current_state);

        printf("Popped: (%s, %d)\n", current_str, current_state);

        
        // Add the string to the solution whenever we reach an accepting state
        if (accepting[current_state] == 1) {

            strcpy(out_strings[found], current_str);
            printf("Found string: \"%s\" \n", out_strings[found]);
            found++;

            if (found == NUM_STR) return out_strings;  
        }


        // Add each transition out of the current state to the queue
        // as long as the strings aren't too long and the queue hasn't been
        // filled.
        if (strlen(current_str) < STRLEN_MAX && !qflush_mode) {
            
            current_node = get_node(g, current_state);
            add_transitions(queue, current_node, current_str);

        }

        print_bqueue(queue);

        free(current_str);

        printf("Iterating...\n\n");
    }

    return out_strings;
}