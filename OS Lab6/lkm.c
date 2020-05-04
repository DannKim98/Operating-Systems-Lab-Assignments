/*
Two semaphores were used for synchronization.
takeSem to control take thread is initialized to 0
giveSem to control give thread is initialized to 1,
so will not wait for the first time.
Data synchronization is not needed,
therefore mutex was not used.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/semaphore.h>

static struct task_struct *give;
static struct task_struct *take;
static struct semaphore giveSem;
static struct semaphore takeSem;

int give_function(void *unused){
	while(!kthread_should_stop()){
		down(&giveSem);
		printk("lkm: give\n");
		ssleep(1);
		up(&takeSem);
	}
	do_exit(0);
	return 0;
}

int take_function(void *unused){
	while(!kthread_should_stop()){
		down(&takeSem);
		printk("lkm: take\n");
		ssleep(1);
		up(&giveSem);
	}
	do_exit(0);
	return 0;
}

static int __init lkm_init(void){
	printk("lkm: kthread init called\n");
	sema_init(&giveSem, 1);
	sema_init(&takeSem, 0);

	//create and wake up a take thread
	take = kthread_run(take_function, NULL, "take_name");
	if (take)
		printk(KERN_INFO "lkm: the take thread was generated firstly\n"); 
	else
		printk(KERN_ERR "lkm: can't create the take thread\n");

	//create and wake up a give thread
	give = kthread_run(give_function, NULL, "give_name");
	if (give)
		printk(KERN_INFO "lkm: the give thread was generated secondly\n"); 
	else
		printk(KERN_ERR "lkm: can't create the give thread\n");

	return 0;
}

static void __exit lkm_exit(void){
	printk("lkm: kthread exit called\n");
	kthread_stop(take);
	kthread_stop(give); 
}

module_init(lkm_init);
module_exit(lkm_exit); 

MODULE_LICENSE("GPL");
