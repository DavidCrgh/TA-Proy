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

    /*b_queue_t *queue = init_bqueue(10);
    char *mystr = "Hola";
    char *outstr = NULL;
    int outstate;

    enqueue(queue, mystr, 5);
    enqueue(queue, mystr, 2);
    enqueue(queue, mystr, 4);
    enqueue(queue, mystr, 6);

    print_bqueue(queue);

    outstr = dequeue(queue, &outstate);
    printf("DQ -> (%s, %d)\n", outstr, outstate);

    print_bqueue(queue);

    enqueue(queue, mystr, 12);
    
    outstr = dequeue(queue, &outstate);
    printf("DQ -> (%s, %d)\n", outstr, outstate);

    enqueue(queue, mystr, 15);

    print_bqueue(queue);*/

    testingStrings();

    return 0;
}
