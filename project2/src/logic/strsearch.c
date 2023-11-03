#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "strsearch.h"
#include "bqueue.h"
#include "common.h"
#include "list.h"
#include "matrix.h"
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "dfa.h"


int *useful_states = NULL;


int *get_useful(graph *g, int *accepting, bool complement) {

    int n_states = complement ? num_states + 1 : num_states;
    int *useful = (int*) createList(n_states, sizeof(int));

    bool changed = true;
    node *current;
    edge *current_e;

    int src_id = -1;
    int dest_id = -1;


    // Accept states are useful by default
    memcpy(useful, accepting, n_states * sizeof(int));


    // Repeat this whenever there's changes on the 'useful' array
    while (changed) {
        changed = false;
        current = g->nodes;

        // Iterate all graph nodes
        while (current != NULL) {

            src_id = current->id - 1;

            // Skip nodes already marked as useful
            if (useful[src_id] == 1) {

                current = current->next;
                continue;
            }


            current_e = current->edges;


            // Iterate current node's edges
            while (current_e != NULL) {

                dest_id = current_e->dest->id - 1;

                // A state is useful if it can reach another useful state
                if (useful[dest_id] == 1) {

                    useful[src_id] = 1;
                    changed = true;
                    current_e = NULL; // Stop iterating edges once we find an useful destination

                } else {

                    current_e = current_e->next;

                }

            }

            current = current->next;
        }
    }


    return useful;
}


// Obtains the complement of the automata's accepting states.
int *flip_accept_states(int *accept) {
    int *new_accept = (int*) malloc(sizeof(int) * (num_states + 1));

    for (int i = 0; i < num_states; i++) {

        new_accept[i] = abs(accept[i] - 1);
    }

    new_accept[num_states] = 1;

    return new_accept;
}


void add_transitions(b_queue_t *q, node *state, char *str, int *accept) {

    edge *current = state->edges;
    char symbol;

    int symbol_idx = 0;

    char *new_string = NULL;
    int next_state = -1;

    while (current != NULL) {

        symbol = current->symbols[symbol_idx];

        while (symbol != '\0') {

            next_state = current->dest->id - 1;


            if (useful_states[next_state] == 1) {

                 // Append transition symbol to the existing string
                new_string = (char *) malloc(sizeof(char) * STRLEN_MAX); // This memory is always freed somewhere else
                strcpy(new_string, str);
                strncat(new_string, &symbol, 1);


                // Add (currentString + nextSymbol, nextState) to the queue
                if (accept[next_state] == 1) {
                
                    prioritize(q, new_string, next_state); // Prioritize (push to front) transitions to accepting states

                } else {
                    enqueue(q, new_string, next_state);
                }
            }


            symbol_idx++;
            symbol = current->symbols[symbol_idx];
        }

        current = current->next;
        symbol_idx = 0;
    }
}


void get_strings(graph *g, machine_conf_t *conf, bool complement, int num_str, char **out_strings, int *out_found) {

    // Some setup required to run the string search
    b_queue_t *queue = init_bqueue(num_str);
    b_queue_t *nxt_queue = init_bqueue(num_str);
    b_queue_t *swap_ptr = NULL;

    int current_state = -1;
    node *current_node = NULL;

    char *current_str = malloc(sizeof(char) * STRLEN_MAX);
    current_str[0] = '\0'; // First string is epsilon ""

    int found = 0;


    int *accepting = NULL;
    

    if (complement) {
        accepting = flip_accept_states(conf->accept);
        add_sink(g, conf->table, conf->symbols);

    } else {
        accepting = conf->accept;
    }


    // Define useful states
    useful_states = get_useful(g, accepting, complement);


    // Algorithm starts
    // Add empty string and initial state to queue
    enqueue(queue, current_str, 0); 
    print_bqueue(queue);


    // Iterate through the queue
    while (queue->count > 0 || nxt_queue->count > 0) {

        current_str = dequeue(queue, &current_state);

        printf("Popped: (%s, %d)\n", current_str, current_state);

        
        // Add the string to the solution whenever we reach an accepting state
        if (accepting[current_state] == 1) {

            strcpy(out_strings[found], current_str);
            printf("Found string: \"%s\" \n", out_strings[found]);
            found++;
            
            dec_limit(queue);
            dec_limit(nxt_queue);

            if (found == num_str) break;  
        }


        // Add each transition out of the current state to the queue
        // as long as the strings aren't too long.
        if (strlen(current_str) < STRLEN_MAX) {
            
            current_node = get_node(g, current_state);
            add_transitions(nxt_queue, current_node, current_str, accepting);

        }

        free(current_str);

        print_bqueue(queue);


        // Swap queues when the current one is empty
        if (queue->count == 0) {
            swap_ptr = queue;
            queue = nxt_queue;
            nxt_queue = swap_ptr;
        }
    }

    // Do some cleanup
    free_bqueue(queue);
    free_bqueue(nxt_queue);
    free(accepting);
    free(useful_states);

    *out_found = found;
}