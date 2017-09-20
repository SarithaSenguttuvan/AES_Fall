#include <stdio.h>
#include <stdlib.h>
#include <linux/kernel.h>
#include <unistd.h>

int main()
{
    size_t max_buff_size = 10;
    size_t i = 0;
    long int syscall_ret_val = 0;
    int32_t *src_buff = (int32_t *)malloc(max_buff_size * sizeof(int32_t));     /* allocating memory for source unsorted buffer */
    int32_t *des_buff = (int32_t *)malloc(max_buff_size * sizeof(int32_t));     /* allocating memory for destination sorted buffer */
    
    int32_t *temp_buff = src_buff;
    if(src_buff == NULL  ||  des_buff == NULL)                  /* checks if pointer is valid */
        return -1; 

    /* First time - syscall */   
    for(i = 0; i<max_buff_size; i++)
    {
        *temp_buff = (int32_t)rand()%500;               /* generating random number within 500 */
        temp_buff++;
    }
    printf("\nThe unsorted buffer is\n");
    for(i = 0; i<max_buff_size; i++)
    {
        printf(" %d, ", *(src_buff + i));           /* printing the unsorted buffer value */
    }   
        
    if(syscall(333, src_buff, max_buff_size, des_buff))     /* syscall for copying to kernel space, sorting and returning the sorted buffer */
        printf("\nError\n");         /* syscall returned value */
    printf("\nThe sorted buffer is\n");
 
    for(i = 0; i<max_buff_size; i++)
    {
        printf(" %d, ", *(des_buff + i));           /* printing the sorted destination buffer */ 
    } 

   temp_buff = src_buff; 
   
    /* Second time - syscall */     
    for(i = 0; i<max_buff_size; i++)
    {
        *temp_buff = (int32_t)rand()%600;               /* generating random number within 500 */
        temp_buff++;
    }
    printf("\nThe unsorted buffer is\n");  
    for(i = 0; i<max_buff_size; i++)
    {
        printf(" %d, ", *(src_buff + i));           /* printing the unsorted buffer value */
    } 
 
    if(syscall(333, src_buff, max_buff_size, des_buff))     /* syscall for copying to kernel space, sorting and returning the sorted buffer */
        printf("\nError\n");         /* syscall returned value */
    printf("\nThe sorted buffer is\n");
 
    for(i = 0; i<max_buff_size; i++)
    {
        printf(" %d, ", *(des_buff + i));           /* printing the sorted destination buffer */ 
    } 

    temp_buff = src_buff;    
        
    /* Third time - syscall - Failure case*/     
    for(i = 0; i<max_buff_size; i++)
    {
        *temp_buff = (int32_t)rand()%50;               /* generating random number within 500 */
        temp_buff++;
    }
    printf("\nThe unsorted buffer is\n");  
    for(i = 0; i<max_buff_size; i++)
    {
        printf(" %d, ", *(src_buff + i));           /* printing the unsorted buffer value */
    }   
    src_buff = NULL;
    if(syscall(333, src_buff, max_buff_size, des_buff))     /* syscall for copying to kernel space, sorting and returning the sorted buffer */
        printf("\nERROR\n");         /* syscall returned value */
        
    printf("\nThe sorted buffer is\n");
 
    for(i = 0; i<max_buff_size; i++)
    {
        printf(" %d, ", *(des_buff + i));           /* printing the sorted destination buffer */ 
    } 
     
    free(src_buff);                             /* deallocating the memory */
    free(des_buff);                             /* deallocating the memory */ 
    return 0;
}
