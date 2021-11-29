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
    posts the fill_count semaphore. The consumer has a problem returning a 
    pointer from kernel space back to user space. The data structure in
    kernel space still works.

Test Files:
    We have a kernel-space test file called "test.c", and a slightly more
    thorough user-space test file called "user_test.c". Testing through the
    more restrictive system calls only allows for rudimentary tests, while the
    user-space tests utilize member variables and more precise edge case setup.


Github:
	Issues uploading all the files to the git using the command line due to
	large files after make clean and mr proper. We instead manually uploaded 
	the required files manually to the git. In include/linux github truncated
	syscalls.h from the list, we are unable to know if it has been replaced
	with our edited file.



How to compile and run tests:

    Run the following commands for each file.

    test.c:
        cd kernel_tests
        make run

    user_test.c:
        cd user_tests
        make run
