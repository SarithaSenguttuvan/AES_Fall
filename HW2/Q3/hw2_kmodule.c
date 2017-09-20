#include <linux/module.h>   
#include <linux/kernel.h>   
#include <linux/init.h>    

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Dummy kernel momdule");
MODULE_AUTHOR("Saritha Senguttuvan");

void ktimer_CB_func(unsigned long data);        /* function prototype for call bcak function for timer */
struct timer_list my_ktimer;                    /* declaring a timerof type struct timer_list */
    
static int kmod_init(void)                      /* function to initialize the kernel module */
{
    printk(KERN_INFO "Loading Kernel \n");
    printk("Timer module installing \n");  
    my_ktimer.expires = jiffies + msecs_to_jiffies(500);        /* giving the timer expiration time as 500 ms */
    my_ktimer.data = 0;                             /* argument passed to the call back function */
    my_ktimer.function = ktimer_CB_func;               /* assigning call back function name */ 
        
    init_timer(&my_ktimer);                         /* initialzing the timer */
    printk(" The jiffies value before call back is %ld \n", jiffies);
    add_timer(&my_ktimer);                          /* starting the timer */
  
    return 0;
}

void ktimer_CB_func(unsigned long data)         /* Timer Call back function */
{
    static size_t count = 1;
    printk(" The jiffies value after call back is %ld \n", jiffies);
    printk("The kernel has run %ld times \n", count);           /* Prints the number of times 500 ms has elapsed */
    count++;
    mod_timer( &my_ktimer, jiffies + msecs_to_jiffies(500));    /* modify the timer value to elapse after 500 ms */
    return;
}

static void kmod_exit(void)             /* function to exit the kernel module */ 
{
    printk(KERN_INFO "unloading kernel\n");      
    del_timer( &my_ktimer );            /* delete the timer */
    printk("Timer module uninstalling\n");
}

module_init(kmod_init);      /* kmod_init is registered as the entry point for the kernel module */   
module_exit(kmod_exit);      /* kmod_exit is registered as the exit point for the kernel module */ 
