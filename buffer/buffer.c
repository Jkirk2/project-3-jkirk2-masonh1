
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include "buffer.h"


static ring_buffer_421_t buffer;
ring_buffer_421_t *buffer_p = &buffer;
static DEFINE_SEMAPHORE (mutex);
static DEFINE_SEMAPHORE (fill_count);
static DEFINE_SEMAPHORE (empty_count);

SYSCALL_DEFINE0(init_buffer_421)
{
	node_421_t *node;
	node_421_t *cur;
	int i;
	// Note: You will need to initialize semaphores in this function.

	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write)
	{
		printk("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	// Create the root node.
	node = (node_421_t *)kmalloc(sizeof(node_421_t),GFP_KERNEL);
	
	// Create the rest of the nodes, linking them all together.
	cur = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++)
	{
		cur->next = (node_421_t *)kmalloc(sizeof(node_421_t),GFP_KERNEL);
		cur = cur->next;
	}
	// Complete the chain.
	cur->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// initialize semaphores
	sema_init(&mutex, 1);					 // set to 1 to avoid hanging
	sema_init(&fill_count, 0);			     // no spaces are filled
	sema_init(&empty_count, SIZE_OF_BUFFER); // all spaces are empty

	return 0;
}

SYSCALL_DEFINE1(enqueue_buffer_421, char *, data)
{
	int rc;
	printk("enqueue with data: ");
	printk(data);
	printk("\n");
	// NOTE: You have to modify this function to use semaphores.

	// check if buffer is uninitialized
	if (!buffer.write)
	{
		printk("enqueue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	down(&empty_count); // decrement empty count
	down(&mutex);

	// put chars from data into write
	//memcpy(buffer.write->data, data, DATA_LENGTH);
	 rc = copy_from_user(buffer.write->data, data, DATA_LENGTH);
	
	// Advance the pointer.
	buffer.write = buffer.write->next;
	// update length
	buffer.length++;

	up(&mutex);
	up(&fill_count); // increment fill count

	return 0;
}

SYSCALL_DEFINE1(dequeue_buffer_421 , char *, data)
{
	int rc;


	
	// check if buffer is uninitialized
	if (!buffer.read)
	{
		printk("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	down(&fill_count); // decrement fill count
	down(&mutex);
	
	printk("buffer data in dequeue: ");
	printk(buffer.read->data);
	printk("\n");
	// put chars from read into data
	// data = buffer.read->data;
	//strcpy(data, buffer.read->data);
	rc = copy_to_user(data, buffer.read->data, DATA_LENGTH);
	
	// move read forward by one
	buffer.read = buffer.read->next;
	// update length
	buffer.length--;
	printk("Dequeue with data: ");
	printk(data);
	printk("\n");
	up(&mutex);
	up(&empty_count); // increment empty count

	return 0;
}

SYSCALL_DEFINE0(delete_buffer_421)
{	
	node_421_t *temp;
	node_421_t *cur;
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read)
	{
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	// Get rid of all existing nodes.
	
	cur = buffer.read->next;
	while (cur != buffer.read)
	{
		temp = cur->next;
		kfree(cur);
		cur = temp;
	}
	// Free the final node.
	kfree(cur);
	cur = NULL;
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	return 0;
}
