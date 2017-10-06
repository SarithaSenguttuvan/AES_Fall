/**
 * @file 
 *		kthread_create.c
 * @brief 
 *		This file implements the profiling for kthread_create
 *
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		Oct,2017
 *
 */
#include <linux/module.h>   
#include <linux/kernel.h>   
#include <linux/init.h>
#include <linux/kthread.h>  
#include <linux/timex.h> 

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("kThread_Create momdule");
MODULE_AUTHOR("Saritha Senguttuvan");

void ktimer_CB_func(unsigned long data);        /* function prototype for call bcak function for timer */
static struct task_struct *tid_1;
 
int dummy_fn(void* arg)
{
    //printk("Do nothing in the child function \n");
    return 0;    
} 
#if 0 
static int kthread_mod_init(void)                      /* function to initialize the kernel module */
{  
    unsigned long jiffies_before;
    unsigned long jiffies_after;
    unsigned int a;
    unsigned int b;
    char  my_kernelThread[8] = "tid_1";
    printk("Current process id is %d\n", current->pid); 
    printk(KERN_INFO "Loading Kernel \n");
    jiffies_before = (unsigned long)get_jiffies_64();       /* Getting CPU time in terms of jiffies */
    tid_1 = kthread_create(&dummy_fn,NULL,my_kernelThread); /* Kthread Create call*/
    jiffies_after = (unsigned long)get_jiffies_64();       /* Getting CPU time in terms of jiffies */ 
    printk("The jiffies_before = %ld\n", jiffies_before);
    printk("The jiffies_after = %ld\n", jiffies_after);  
    a = jiffies_to_msecs(jiffies_before);
    b = jiffies_to_msecs(jiffies_after);
    printk("The new jiffies value is %u \n", a);
    printk("The new jiffies value is %u \n", b);
    return 0;
}
#endif
static int kthread_mod_init(void)                      /* function to initialize the kernel module */
{  
    cycles_t cycles_before;
    cycles_t cycles_after;
    char  my_kernelThread[8] = "tid_1";
    cycles_before = get_cycles();
    tid_1 = kthread_create(&dummy_fn,NULL,my_kernelThread); /* Kthread Create call*/
    cycles_after = get_cycles();
    printk("The cycles before is %llu \n",cycles_before); 
    printk("The cycles after is %llu \n",cycles_after); 
    printk("The number of CPU cycles for Kthread_create is %llu \n", (cycles_after - cycles_before)); 
    return 0;
}
static void kthread_mod_exit(void)             /* function to exit the kernel module */ 
{
    printk(KERN_INFO "unloading kernel\n"); 
    if(tid_1)
        kthread_stop(tid_1);     
}

module_init(kthread_mod_init);      /* kmod_init is registered as the entry point for the kernel module */   
module_exit(kthread_mod_exit);      /* kmod_exit is registered as the exit point for the kernel module */ 
