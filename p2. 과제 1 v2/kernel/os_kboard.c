#include <linux/syscalls.h>
#include <linux/printk.h>
#include <linux/uaccess.h>
#define MAX_CLIP (6)

int ring[MAX_CLIP];
int rear = 0;
int front = 0;

long do_sys_kb_enqueue(int item) {

	if(item > 0) {
		if((rear + 1) % MAX_CLIP == front) {
			printk(KERN_DEBUG "HCPARK: do_sys_kb_enqueue() CALLED!! clip=%d\n", item);
			printk(KERN_DEBUG "HCPARK: do_sys_kb_enqueue() ring is full!\n");

			return -1;
		}

		else {
			rear = (rear+1) % MAX_CLIP;
			ring[rear] = item;
			
			printk(KERN_DEBUG "HCPARK: do_sys_kb_enqueue() CALLED!! clip=%d\n", item);

			return 0;
		}
	}
	else {
		printk(KERN_DEBUG "HCPARK: do_sys_kb_enqueue() CALLED!! clip=%d\n", item);
		printk(KERN_DEBUG "HCPARK: do_sys_kb_enqueue() invalid integer clip=%d\n", item);
		return -2;
	}
}

long do_sys_kb_dequeue(int *user_buf) {
	
	if(rear == front) {
		printk(KERN_DEBUG "HCPARK: do_sys_kb_dequeue() CALLED!!\n");
		printk(KERN_DEBUG "HCPARK: do_sys_kb_dequeue() ring is empty!!\n");
		return -1;
	}
	
	else {
		front = (front + 1) % MAX_CLIP;

		printk(KERN_DEBUG "HCPARK: do_sys_kb_dequeue() CALLED!!\n");
		copy_to_user(user_buf, &ring[front], sizeof(ring[front]));
	
		return 0;
	}
}

SYSCALL_DEFINE1(kb_enqueue, int, item) {
	return do_sys_kb_enqueue(item);
}

SYSCALL_DEFINE1(kb_dequeue, int __user *, user_buf) {
	return do_sys_kb_dequeue(user_buf);
}
