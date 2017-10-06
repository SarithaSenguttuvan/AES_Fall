/**
 * @file 
 *		find_kthreads.c
 * @brief 
 *		This file finds the number of children for each process and finds the init process by traversing
 *
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		Oct,2017
 *
 *  NOTE: I have referred the sched.h at path /home/saritha/Fall_2017/AES/linux-4.12.10/include/linux/sched.h
 *        list.h at path /home/saritha/Fall_2017/AES/linux-4.12.10/include/linux/list.h
 */

#include <linux/module.h>   
#include <linux/kernel.h>   
#include <linux/init.h> 
#include <linux/sched.h>   
#include <linux/sched/task.h> 

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Find kernel threads module");
MODULE_AUTHOR("Saritha Senguttuvan");

static int find_process_init(void)                      /* function to initialize the kernel module */
{
    size_t children_number = 0;
    struct task_struct *my_current_task;
    struct list_head *traverse_task_list;
    long int current_state;
    printk(KERN_INFO "Loading find Kernel threads module \n");   
    for(my_current_task=current; my_current_task != &init_task; my_current_task = my_current_task->parent)
    {   
        printk("Current process id is %d\n", my_current_task->pid);               /* print the current processes pid */
        current_state = my_current_task->state;
        printk("Current process state is %ld\n", my_current_task->state);               /* print the current processes state */
        if(current_state == -1)
            printk("The state is UNRUNNABLE!\n");
        else if(current_state == 0)
            printk("The state is RUNNABLE!\n");   
        else
            printk("The state is STOPPED!\n"); 
        printk("Current thread name is %s\n",my_current_task->comm);           /* current thread's name */ 
        
        list_for_each(traverse_task_list, &my_current_task->children)
        {
            children_number++;
        }
        printk("Number of children for the current pid %d is %ld", my_current_task->pid, children_number); 
        children_number = 0;
    }
    printk("The init process has been reached\n");
    printk("Init process id is %d\n", my_current_task->pid);               /* print the current processes pid */
    current_state = my_current_task->state;
    printk("Init process state is %ld\n", my_current_task->state);               /* print the current processes state */
    if(current_state == -1)
       printk("The init process state is UNRUNNABLE!\n");
    else if(current_state == 0)
       printk("The init process state is RUNNABLE!\n");   
    else
       printk("The init process state is STOPPED!\n"); 
    printk("Init thread name is %s\n",my_current_task->comm);           /* current thread's name */     

    list_for_each(traverse_task_list, &my_current_task->children)       /*Function call to traverse through the children */ 
    {
        children_number++;                                          /* increment the number of children */
    }
    printk("Number of children for the init process pid %d is %ld\n", my_current_task->pid, children_number);  /* Prints the number of children for the init process */
    children_number = 0;
    return 0;
}

static void find_process_exit(void)             /* function to exit the kernel module */ 
{
    printk(KERN_INFO "unloading find Kernel threads module\n"); 
}
module_init(find_process_init);      /* find_process_init is registered as the entry point for the kernel module */   
module_exit(find_process_exit);      /* find_process_exit is registered as the exit point for the kernel module */ 

