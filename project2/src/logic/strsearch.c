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


// Obtains the complement of the automata's accepting states.
int *flip_accept_states(int *accept) {
    int *new_accept = (int*) malloc(sizeof(int) * num_states);

    for (int i = 0; i < num_states; i++) {

        new_accept[i] = abs(accept[i] - 1);
    }

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

            // Append transition symbol to the existing string
            new_string = (char *) malloc(sizeof(char) * STRLEN_MAX); // This malloc is paired with free(current_str) in the get_strings function
            strcpy(new_string, str);
            strncat(new_string, &symbol, 1);

            next_state = current->dest->id - 1;

            
            // Add (currentString + nextSymbol, nextState) to the queue
            if (accept[next_state] == 1) {
                
                prioritize(q, new_string, next_state); // Prioritize (push to front) transitions to accepting states

            } else {
                enqueue(q, new_string, next_state);
            }


            symbol_idx++;
            symbol = current->symbols[symbol_idx];
        }

        current = current->next;
        symbol_idx = 0;
    }
}


void get_strings(graph *g, int *accept, bool complement, int num_str, char **out_strings, int *out_found) {

    // Some setup required to run the string search
    b_queue_t *queue = init_bqueue(num_str);
    b_queue_t *nxt_queue = init_bqueue(num_str);
    b_queue_t *swap_ptr = NULL;

    int current_state = -1;
    node *current_node = NULL;

    char *current_str = malloc(sizeof(char) * STRLEN_MAX);
    current_str[0] = '\0'; // First string is epsilon ""

    int found = 0;

    int *accepting = complement ? flip_accept_states(accept) : accept;


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

    printf("Freeing queues... \n\n");
    free_bqueue(queue);
    free_bqueue(nxt_queue);

    *out_found = found;
}