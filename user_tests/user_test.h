#ifndef USER_TEST_H
#define USER_TEST_H

#include <stdbool.h>
#include <time.h>
#include <errno.h>

#ifndef STRUCT_TIMESPEC_H
#define STRUCT_TIMESPEC_H
typedef const struct timespec_t {
    time_t tv_sec; // num seconds
    long tv_nsec; // num nanoseconds
}timespec;
#endif

void *producer(char* data);
void *consumer(char* data);

bool test1_init();
bool test2_delete();
bool test3_enqueue();
bool test4_dequeue();
bool test5_stress();

void wait_random();

void ensure_init();
void ensure_delete();

long init_buffer_syscall();
long delete_buffer_syscall();
long enqueue_buffer_syscall(char* data);
long dequeue_buffer_syscall(char* data);

#endif
