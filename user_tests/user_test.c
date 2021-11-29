#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "utils.h"
#include "buffer.h"
#include "user_test.h"

// the function calls are in syscall format so that
// transfering to syscalls in kernel space will be easier

long init_buffer_syscall(){
    return init_buffer_421();
}

long delete_buffer_syscall(){
    return delete_buffer_421();
}

long enqueue_buffer_syscall(char* data){
    return enqueue_buffer_421(data);
}

long dequeue_buffer_syscall(char* data){
    return dequeue_buffer_421(data);
}

void *producer(char* data)
{
    printf("----------- Adding 10,000 items to buffer--------------\n");

    for (int i = 0; i < 10000; i++)
    {

        snprintf(data, sizeof(data), "%d", i);
        printf("enqueing: ");
        wait_random();
        print_semaphores();
        enqueue_buffer_syscall(data);
    }
}

void *consumer(char* data)
{
    printf("----------- dequeue 10,000 items from buffer--------------\n");

    for (int j = 0; j < 10000; j++)
    {
        printf("dequeueing: ");
        wait_random();
        print_semaphores();
        dequeue_buffer_syscall(data);
    }
}

bool test1_init(){
    printf("NOW TESTING init_buffer_421().\n");
    bool is_passing = true;

    // init regularly
    ensure_delete();
    if (init_buffer_syscall() != 0){
        is_passing = false;
    }

    // init duplicate
    ensure_init();
    if (init_buffer_syscall() != -1){
        is_passing = false;
    }

    if (is_passing){
        printf("Passed test1_init()!\n");
    }
    else {
        printf("Failed test1_init()!\n");
    }

    sleep(2); // wait so user can see result
    return is_passing;
}

bool test2_delete(){
    printf("NOW TESTING delete_buffer_421().\n");
    bool is_passing = true;

    // delete twice
    ensure_delete();
    if (delete_buffer_syscall() != -1){
        is_passing = false;
    }

    // delete regularly
    ensure_init();
    if (delete_buffer_syscall() != 0){
        is_passing = false;
    }

    if (is_passing){
        printf("Passed test2_delete()!\n");
    }
    else {
        printf("Failed test2_delete()!\n");
    }

    sleep(2); // wait so user can see result
    return is_passing;
}
bool test3_enqueue(){
    printf("NOW TESTING enqueue_buffer_421(char* data).\n");
    bool is_passing = true;

    char data[DATA_LENGTH];

    // enqueue to uninit buffer
    ensure_delete();
    if (enqueue_buffer_syscall(data) != -1){
        is_passing = false;
    }

    // enqueue to empty buffer
    ensure_init();
    empty_buffer();
    set_string(data);
    if (enqueue_buffer_syscall(data) != 0){
        is_passing = false;
    }

    // enqueue to somewhat full buffer
    ensure_init();
    empty_buffer();
    for (int ii = 0; ii < SIZE_OF_BUFFER / 2; ii++){
        enqueue_buffer_syscall(data);
    }
    if (enqueue_buffer_syscall(data) != 0){
        is_passing = false;
    }

    // test enqueue to full buffer during stress test

    if (is_passing){
        printf("Passed test3_enqueue()!\n");
    }
    else {
        printf("Failed test3_enqueue()!\n");
    }

    sleep(2); // wait so user can see result
    return is_passing;
}

bool test4_dequeue(){
    printf("NOW TESTING dequeue_buffer_421(char* data).\n");
    bool is_passing = true;
    char data[DATA_LENGTH];

    // dequeue to uninit buffer
    ensure_delete();
    if (dequeue_buffer_syscall(data) != -1){
        is_passing = false;
    }

    // dequeue to empty buffer during stress test

    // dequeue to somewhat full buffer
    ensure_init();
    empty_buffer();
    for (int ii = 0; ii < SIZE_OF_BUFFER / 2; ii++){
        enqueue_buffer_syscall(data);
    }
    if (dequeue_buffer_syscall(data) != 0){
        is_passing = false;
    }

    // dequeue to full buffer
    ensure_init();
    fill_buffer();
    if (dequeue_buffer_syscall(data) != 0){
        is_passing = false;
    }

    if (is_passing){
        printf("Passed test4_dequeue()!\n");
    }
    else {
        printf("Failed test4_dequeue()!\n");
    }

    sleep(2); // wait so user can see result
    return is_passing;
}

bool test5_stress(){
    printf("NOW TESTING producer/consumer functionality.\n");
    bool is_passing = true;
    char data[DATA_LENGTH];

    // stress test buffer using pthreads for producer/consumer
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, (void*) producer, data);
    pthread_create(&thread2, NULL, (void*) consumer, data);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // if made it this far, passed
    printf("Passed test5_stress()!\n");

    sleep(2); // wait so user can see result
    return is_passing;
}

// waits 0-10 milliseconds
void wait_random(){
    // 10 milliseconds == 10^7 nanoseconds

    int num_nanoseconds = rand() % 10000001;
    timespec time = {.tv_sec = 0, .tv_nsec = num_nanoseconds};
    const timespec* time_p = &time;

    nanosleep(time_p, NULL);
}

void ensure_init(){
    if (!buffer_p->read || !buffer_p->write){
        init_buffer_syscall();
    }
}

void ensure_delete(){
    if (buffer_p->read || buffer_p->write){
        delete_buffer_syscall();
    }
}

int main()
{
    srand(time(NULL));

    if (test1_init() && test2_delete() && test3_enqueue() && test4_dequeue() && test5_stress()){
        printf("ALL TESTS PASSED!\n");
        return 0;
    }
    else {
        printf("FAILED AT LEAST ONE TEST!\n");
        return -1;
    }

    ensure_delete();
}
