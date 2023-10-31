#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ui/setup_ui.h"
#include "test/test.h"
#include "logic/bqueue.h"

int main(int argc, char **argv) 
{
    //init_gui(argc, argv);
    //testingGraph();

    /*b_queue_t *queue = init_bqueue(6);
    char *mystr = (char*) malloc(sizeof(char) * 10);
    char *outstr = NULL;
    int outstate;

    prioritize(queue, mystr, 5);
    print_bqueue(queue);
    prioritize(queue, mystr, 10);
    print_bqueue(queue);
    prioritize(queue, mystr, 15);
    print_bqueue(queue);

    enqueue(queue, mystr, 1);
    enqueue(queue, mystr, 2);
    enqueue(queue, mystr, 3);
    print_bqueue(queue);

    mystr = (char*) malloc(sizeof(char) * 10);
    prioritize(queue, mystr, 5);
    print_bqueue(queue);
    mystr = (char*) malloc(sizeof(char) * 10);
    prioritize(queue, mystr, 10);
    print_bqueue(queue);
    mystr = (char*) malloc(sizeof(char) * 10);
    prioritize(queue, mystr, 15);
    print_bqueue(queue);

    enqueue(queue, mystr, 5);
    enqueue(queue, mystr, 2);
    //enqueue(queue, mystr, 4);
    //enqueue(queue, mystr, 6);

    print_bqueue(queue);


    prioritize(queue, mystr, 100);

    print_bqueue(queue);

    outstr = dequeue(queue, &outstate);
    printf("DQ -> (%s, %d)\n", outstr, outstate);
    

    print_bqueue(queue);

    outstr = dequeue(queue, &outstate);
    printf("DQ -> (%s, %d)\n", outstr, outstate);

    outstr = dequeue(queue, &outstate);
    printf("DQ -> (%s, %d)\n", outstr, outstate);

    print_bqueue(queue);

    printf("Done\n");*/

    testingStrings();

    return 0;
}
