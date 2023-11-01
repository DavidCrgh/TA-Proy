#ifndef BQUEUE_H
#define BQUEUE_H

typedef struct bq_node_t bq_node_t;
typedef struct b_queue_t b_queue_t;


struct bq_node_t {

    char *str;
    int state;
    bq_node_t *next;
    bq_node_t *prev;

};

struct b_queue_t {

    int limit;
    int count;
    bq_node_t *front;
    bq_node_t *back;

};

b_queue_t *init_bqueue(int limit);
int enqueue(b_queue_t *queue, char *str, int state);
int prioritize(b_queue_t *queue, char *str, int state);
char *dequeue(b_queue_t *queue, int *retstate);
int dec_limit(b_queue_t *queue);
void print_bqueue(b_queue_t *queue);
void free_bqueue(b_queue_t *queue);

#endif // BQUEUE_H