#include <stdlib.h>
#include <stdio.h>

#include "bqueue.h"
#include "common.h"

b_queue_t *init_bqueue(int limit) {
    b_queue_t *queue = malloc(sizeof(b_queue_t));
    queue->limit = limit;
    queue->count = 0;
    queue->front = NULL;
    queue->back = NULL;


    return queue;
}

int enqueue(b_queue_t *queue, char *str, int state) {

    if (queue->count == queue->limit) return FAIL;

    bq_node_t *new_node = malloc(sizeof(bq_node_t));
    new_node->str = str;
    new_node->state = state;
    new_node->next = NULL;


    if (queue->count == 0) {
        queue->front = new_node;
        queue->back = new_node;

    } else {
        queue->back->next = new_node;
        queue->back = new_node;
    }

    queue->count++;

    return OK;
}

char *dequeue(b_queue_t *queue, int *retstate){
    
    if (queue->count <= 0) return NULL;

    bq_node_t *current = queue->front;
    
    char *outstr = current->str;
    *retstate = current->state;

    if (queue->count == 1) {
        queue->front = NULL;
        queue->back = NULL;

    } else {
        queue->front = queue->front->next;
    }

    free(current);

    queue->count--;
    
    return outstr;
}

void print_bqueue(b_queue_t *queue) {
    bq_node_t *current = queue->front;

    printf("\n\nQueue:\n");

    if (queue->count == 0) printf("EMPTY!\n\n");

    while (current != NULL) {
        printf("(%s, %d)\n", current->str, current->state);
        current = current->next;
    }
}

void free_bqueue(b_queue_t *queue) {
    // TODO: implement
}