#include <stdlib.h>
#include <stdio.h>

#include "bqueue.h"
#include "common.h"


////////// PRIVATE FUNCTIONS //////////////////////////////////////////////////

bq_node_t *init_bqnode(char *str, int state) {

    bq_node_t *new_node = malloc(sizeof(bq_node_t));
    new_node->str = str;
    new_node->state = state;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}


void delete_last(b_queue_t *queue) {

    if (queue->count == 1) {
        
        free(queue->front->str);
        free(queue->front);
        queue->front = NULL;
        queue->back = NULL;

    } else {

        bq_node_t *temp = queue->back;
        queue->back = temp->prev;
        queue->back->next = NULL;
        free(temp->str);
        free(temp);

    }

    queue->count--;
}


////////// PUBLIC FUNCTIONS ///////////////////////////////////////////////////

b_queue_t *init_bqueue(int limit) {
    b_queue_t *queue = malloc(sizeof(b_queue_t));
    queue->limit = limit;
    queue->count = 0;
    queue->front = NULL;
    queue->back = NULL;


    return queue;
}


int enqueue(b_queue_t *queue, char *str, int state) {

    if (queue->count == queue->limit) {
        free(str);
        return FAIL;
    }

    bq_node_t *new_node = init_bqnode(str, state);


    if (queue->count == 0) {
        queue->front = new_node;
        queue->back = new_node;

    } else {
        new_node->prev = queue->back;
        queue->back->next = new_node;
        queue->back = new_node;
    }

    queue->count++;

    return OK;
}


int prioritize(b_queue_t *queue, char *str, int state) {

    // Never force nodes onto a queue with 0 capacity
    if (queue->limit == 0) return FAIL;
    

    // If the queue is full, we remove the last element to accommodate the new node
    if (queue->count == queue->limit) delete_last(queue);


    // Append new node to start
    if (queue->count == 0) return enqueue(queue, str, state);

    bq_node_t *new_node = init_bqnode(str, state);

    queue->front->prev = new_node;
    new_node->next = queue->front;
    queue->front = new_node;

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
        queue->front->next->prev = NULL;
        queue->front = queue->front->next;
    }

    free(current);

    queue->count--;
    
    return outstr;
}


// Safely decreases the queue limit by 1. If the queue is at max capacity, the last node
// will be deleted.
int dec_limit(b_queue_t *queue) {

    if (queue->limit == 0) return FAIL;

    if (queue->count == queue->limit) delete_last(queue);

    queue->limit--;

    return OK;
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
    
    bq_node_t *current = queue->front;
    bq_node_t *temp = NULL;

    while (current != NULL) {

        temp = current;
        free(temp->str);
        current = temp->next;
        free(temp);
    }

    free(queue);
}