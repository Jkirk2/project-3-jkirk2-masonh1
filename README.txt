Authors: Mason Herndon, James Kirk
Class: CMSC 421 - Operating Systems
Assignment: Project #3, bounded buffer in kernel space

Buffer:
    The buffer is implemented as a circular buffer of size twenty, with
    each element holding a c-string of 1024 chars. The buffer handles
    any number of consecutive inputs/outputs by using semaphores to limit
    enqueueing and dequeueing until the the neccessary resources are available.
    A mutex ensures enqueue and dequeue are atomic.

Producer:
    The producer is implemented as a pthread that waits until the buffer has
    an empty slot, then atomically enqueues. The empty_count semaphore ensures
    that the producer will only enqueue when the buffer can handle another
    element, and that the thread will hang until the consumer thread calls
    dequeue and posts the empty_count semaphore.

Consumer:
    The consumer is implemented as a pthread that waits until the buffer has a
    filled slot, then atomically dequeues. The fill_count semaphore ensures
    that the consumer will only dequeue when the buffer has an element to give,
    and that the thread will hang until the producer thread calls enqueue and
    posts the fill_count semaphore.

Test Files:
    We have a kernel-space test file called "test.c", and a slightly more
    thorough user-space test file called "user_test.c". Testing through the
    more restrictive system calls only allows for rudimentary tests, while the
    user-space tests utilize member variables and more precise edge case setup.

How to compile and run tests:

    Run the following commands for each file.

    test.c:
        cd kernel_tests
        make run

    user_test.c:
        cd user_tests
        make run
