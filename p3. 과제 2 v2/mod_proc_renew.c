#include <linux/slab.h>
#include <linux/module.h>       /* for module programming */
#include <linux/mutex.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

extern struct mutex foo_lock;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Youngil, Moon <jake9999@dreamwiz.com>");
MODULE_DESCRIPTION("A sample driver");
MODULE_LICENSE("Dual BSD/GPL");

/*--------------------------------------------------------*/
/* 1) Generate sample data                                */
/*--------------------------------------------------------*/

#define MAX_CLIP 20

DEFINE_MUTEX(foo_lock);

int ring[MAX_CLIP];
int rear = 0;
int front  = 0;
int read_count = 0;
int write_count = 0;

static long enqueue(int item)
{
	write_count++;

	if(item > 0) {
		if((rear + 1) % MAX_CLIP == front)
			return -1;

		else {
			rear = (rear + 1) % MAX_CLIP;
			ring[rear] = item;

			return 0;
		}
	}
	else
		return -2;
}

static long dequeue(int *user_buf)
{
	write_count++;

	if(rear == front)
		return -1;
	else {
		front = (front + 1) % MAX_CLIP;

		copy_to_user(user_buf, &ring[front], sizeof(ring[front]));
		return 0;
	}
}

static int result_data(int *clip)
{
	int i = 0;

	mutex_lock(&foo_lock);
	clip[i] = enqueue(1);
	i++;

	dequeue(clip);
	mutex_unlock(&foo_lock);

	return 0;
}

static int add_sample_data(void)
{
	if (result_data(ring))
		return -ENOMEM;

	return 0;
}

static int remove_sample_data(void)
{
	int i;

	for(i = 0; i<MAX_CLIP; i++) {
		ring[i] = 0;
	}

	return 0;
}

/*--------------------------------------------------------*/
/* 2) proc operations part                                */
/*--------------------------------------------------------*/

static int foo_simple_show(struct seq_file *s, void *unused)
{
	seq_printf(s, "testEnqueue\n");
	seq_printf(s, "testDequeue\n");

	seq_printf(s, "testRead\n");

	return 0;
}

static int foo_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, foo_simple_show, NULL);
}

static const struct file_operations foo_proc_ops = {
	.owner		= THIS_MODULE,
	.open		= foo_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};


/*--------------------------------------------------------*/
/* 3) proc interface part  (/proc/foo-dir/foo)            */
/*--------------------------------------------------------*/

#define FOO_DIR "kboard"
#define WRITER "writer"
#define READER "reader"
#define COUNT "count"
#define DUMP "dump"

static struct proc_dir_entry *foo_proc_dir = NULL;
static struct proc_dir_entry *foo_proc_write = NULL;
static struct proc_dir_entry *foo_proc_read = NULL;
static struct proc_dir_entry *foo_proc_count = NULL;
static struct proc_dir_entry *foo_proc_dump = NULL;

int foo_proc_init(void)
{
	foo_proc_dir = proc_mkdir(FOO_DIR, NULL);
	
	if (foo_proc_dir == NULL)
	{
		printk("Unable to create /proc/%s\n", FOO_DIR);
								                
		return -1;
	}

	foo_proc_write = proc_create(WRITER, 0, foo_proc_dir, &foo_proc_ops); /* S_IRUGO */
	foo_proc_read = proc_create(READER, 0, foo_proc_dir, &foo_proc_ops);
	foo_proc_count = proc_create(COUNT, 0, foo_proc_dir, &foo_proc_ops);
	foo_proc_dump = proc_create(DUMP, 0, foo_proc_dir, &foo_proc_ops);
	
	if (foo_proc_write == NULL || foo_proc_read == NULL || foo_proc_count == NULL || foo_proc_dump == NULL)
	{
		if(foo_proc_write == NULL)
			printk("Unable to create /proc/%s/%s\n", FOO_DIR, WRITER);

		if(foo_proc_read == NULL)
			printk("Unable to create /proc/%s/%s\n", FOO_DIR, READER);

		if(foo_proc_count == NULL)
			printk("Unable to create /proc/%s/%s\n", FOO_DIR, COUNT);

		if(foo_proc_dump == NULL)
			printk("Unable to create /proc/%s/%s\n", FOO_DIR, DUMP);
										                
		remove_proc_entry(FOO_DIR, NULL);
		
		return -1;										
	}
					      
	return 0;
}

void foo_proc_exit(void)
{
	/* remove directory and file from procfs */
	remove_proc_subtree(FOO_DIR, NULL);

	/* remove proc_dir_entry instance */
	proc_remove(foo_proc_write);
	proc_remove(foo_proc_read);
	proc_remove(foo_proc_count);
	proc_remove(foo_proc_dump);
	proc_remove(foo_proc_dir);

	printk("reader_%d, writer_%d", read_count, write_count);
}

/*--------------------------------------------------------*/
/* 2) Module part                                         */
/*--------------------------------------------------------*/

static int __init foo_init(void)
{
	if (add_sample_data())
	{
		printk(KERN_INFO "add_sample_data() failed.\n");
		return -ENOMEM;								 
	}

	return foo_proc_init();
}

static void __exit foo_exit(void)
{
	remove_sample_data();
	foo_proc_exit();

	return;
}

module_init(foo_init);
module_exit(foo_exit);
