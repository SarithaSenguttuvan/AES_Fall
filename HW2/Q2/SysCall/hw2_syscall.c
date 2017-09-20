#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/gfp.h>

SYSCALL_DEFINE3(buff_sort, int32_t*, buff_src, size_t, buff_size, int32_t*, buff_destn)
{
    long err = 0;
    size_t i = 0;
    size_t j = 0;
    int32_t temp_value = 0;
    int32_t *k_buffer = (int32_t *)kmalloc(buff_size * sizeof(int32_t), GFP_KERNEL);        /* allocating memory in the kernel space */
    int32_t *temp_buff = k_buffer;              /* Using a temporary pointer to kernel buffer */
    printk("The syscall has been entered\n");
    
    if(buff_src == NULL)                    /* check if the source buffer is NULL */
        return -EBADR;                      /* Invalid request descriptor - error number 53*/
                
    if(buff_destn == NULL)
        return -EBADR;                      /* Invalid request descriptor - error number 53*/
        
    if(buff_size <= 0)
         return -EBADR;                      /* Invalid request descriptor - error number 53*/   
                
    if(k_buffer == NULL || temp_buff == NULL)
        return -EBADR;                      /* Invalid request descriptor - error number 53*/ 
    //if (!capable(CAP_SYS_BOOT))             /* Checks if caller has ability to modify */
       // return -EPERM;                      /* return error number 1 */
        
    printk("The size of the buffer is %ld\n", buff_size);
    
    if(copy_from_user(k_buffer, buff_src, (buff_size*sizeof(int32_t))))     /* copy the buffer values from userspace to the kernel space */  
        return -EFAULT;                     /* return error number 14 */
        
    printk("The kernel buffer before sorting is\n"); 
     
    printk(" Sorting began \n");                /* performing a bubble sort */
    for(i = 0; i < buff_size-1; i++) 
    {
       for(j = 0; j < buff_size-1-i; j++) 
       { 
            if(*(temp_buff+j) < *(temp_buff+j+1))       /* swap */
            {
                 temp_value = *(temp_buff+j);
                 *(temp_buff+j) = *(temp_buff+j+1);
                 *(temp_buff+j+1) = temp_value;
            }
       }
    }   
  
    printk(" Sorting has been completed \n");

    if(copy_to_user(buff_destn, k_buffer, (buff_size*sizeof(int32_t))))         /* copy the sorted buffer to the user space */
        return -EFAULT;                     /* return error number 14 */
    kfree(k_buffer);        /* copy the kernel buffer */
    
    printk("Syscall exit\n");
    return err;
}
