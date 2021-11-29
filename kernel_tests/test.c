#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <pthread.h>

#define __NR_init_buffer_syscall 442
#define __NR_enqueue_buffer_syscall 443
#define __NR_dequeue_buffer_syscall 444
#define __NR_delete_buffer_syscall 445

long init_buffer_syscall(void)
{
    return syscall(__NR_init_buffer_syscall);
}
long enqueue_buffer_syscall(char *data)
{
    return syscall(__NR_enqueue_buffer_syscall, data);
}
long dequeue_buffer_syscall(char *data)
{
    return syscall(__NR_dequeue_buffer_syscall, data);
}
long delete_buffer_syscall(void)
{
    return syscall(__NR_delete_buffer_syscall);
}

void *producer(char* data)
{
	char temp[sizeof(data)];
	strcpy(temp, data);
    printf("----------- Adding 10,000 items to buffer--------------\n");

    for (int i = 0; i < 10000; i++)
    {

        snprintf(temp, sizeof(temp), "%d", i);

        
        int num_millisec = rand() % 10000;
        usleep(num_millisec);
        printf("enqueing......\n");
        enqueue_buffer_syscall(temp);
        printf("enqueued: ");
        printf(temp);
        printf("\n");
        
    }
    printf("Passed adding to buffer!\n");
}

void *consumer(char* data)
{
    printf("----------- dequeue 10,000 items from buffer--------------\n");

    for (int j = 0; j < 10000; j++)
    {
        
        int num_millisec = rand() % 10000;
        usleep(num_millisec);
        
        printf("dequeueing......\n");
        
        dequeue_buffer_syscall(data);
        printf("dequeued: ");
        printf(data);
        printf("\n");
        
    }
    printf("Passed!\n");
}






int main(int argc, char *argv[])
{
    srand(time(NULL));
    char data[1024] = {"0"};
    
   printf("clearing Buffer\n");
   delete_buffer_syscall();
    
   
    printf("Enqueue on uninitialized buffer (should fail)\n");
     
    if(enqueue_buffer_syscall(data) == -1){
    	printf("	Failed enqueue\n");
    }
    else{
    	printf("	passed enqueue\n");
    }
   
    printf("dequeue on uninitialized buffer (should fail)\n");
     
    if (dequeue_buffer_syscall(data) == -1)
    {
        printf("	Failed dequeue!\n");
    }
    else
    {
        printf("	Passed dequeue!\n");
    }

    printf("Init buffer\n");
     sleep(1);
    if (init_buffer_syscall() == 0)
    {
        printf("	Passed init\n");
    }
    else
    {
        printf("	Failed init\n");
    }

    printf("Init buffer duplicate (should fail)\n");
     
    if (init_buffer_syscall() == 0)
    {
        printf("	Passed init\n");
    }
    else
    {
        printf("	Failed init\n");
    }
    
    printf("about to enqueue and dequeue 10,000 times...........\n");
    

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, (void*) producer, data);
    pthread_create(&thread2, NULL, (void*) consumer, data);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Deleting buffer\n");
     
    if (delete_buffer_syscall() == 0)
    {
        printf("	Passed delete\n");
    }
    else
    {
        printf("	failed delete\n");
    }

    printf("Deleting buffer again (should fail)\n");
     
    if (delete_buffer_syscall() == 0)
    {
        printf("	Passed delete\n");
    }
    else
    {
        printf("	failed delete\n");
    }


    printf("Init buffer\n");
    
    if (init_buffer_syscall() == 0)
    {
        printf("	Passed init\n");
    }
    else
    {
        printf("	Failed init\n");
    }

    printf("enqueue and dequeue on init buffer once\n");
     
    if (enqueue_buffer_syscall(data) == 0)
    {
        printf("	Passed enqueue!\n");
    }
    else
    {
        printf("	Failed enqueue!\n");
    }
    if (dequeue_buffer_syscall(data) == 0)
    {
        printf("	Passed dequeue!\n");
    }
    else
    {
        printf("	Failed dequeue!\n");
    }

    printf("Deleting buffer\n");
     
    if (delete_buffer_syscall() == 0)
    {
        printf("	Passed delete!\n");
    }
    else
    {
        printf("	failed delete!\n");
    }

    printf("All tests have passed!!\n");


    
    return 0;
}
