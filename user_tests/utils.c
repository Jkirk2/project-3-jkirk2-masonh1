#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// print the current buffer elements
void print_buffer(){
    // check if uninitialized
    if (buffer_p->read == NULL){
        printf("print_buffer failed! Buffer not initialized.\n");
        return;
    }

    node_421_t* cur_node = buffer_p->read;
    printf("Printing Buffer Entries.\n");
    for (int ii = 0; ii < buffer_p->length; ii++){
        printf("Entry: \t%s\n", cur_node->data);
        cur_node = cur_node->next;
    }
}

// fill the buffer to capacity
void fill_buffer(){
    // check if uninitialized
    if (buffer_p->read == NULL){
        printf("fill_buffer failed! Buffer not initialized.\n");
        return;
    }

    // generate random string
    char data[DATA_LENGTH];
    set_string(data);

    // enqueue until full
    while (buffer_p->length < SIZE_OF_BUFFER){
        enqueue_buffer_421(data);
    }
}

// remove all the buffer elements
void empty_buffer(){
    // check if uninitialized
    if (buffer_p->read == NULL){
        printf("empty_buffer failed! Buffer not initialized.\n");
        return;
    }

    char data[DATA_LENGTH];

    // dequeue until empty
    while (buffer_p->length > 0){
        dequeue_buffer_421(data);
    }
}

// set string to a single repeated random digit
void set_string(char* data){
    int num = rand() % 9 + 1;
    for (int ii = 0; ii < DATA_LENGTH; ii++){
        data[ii] = num;
    }
}
