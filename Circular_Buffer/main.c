/**
 * @file 
 *		main.c 
 * @brief 
 *		In this file a call to the main function is made which in turn makes function calls to test circular buffer functioanalities
 * 		  
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		Sept 9,2017
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "circbuf.h"

CB_t * CircBufferPtr;

int main(){
	CB_status buffer_status;
	size_t buffer_length = 5;
	uint32_t data_add = 7;
	uint32_t temp_data = 0;
	size_t no_of_items = 5;
	size_t i = 0;
	buffer_status = CB_init(&CircBufferPtr,buffer_length);
	while(i<no_of_items)
	{
		CB_buffer_add_item(CircBufferPtr, data_add);		/*Adding items to the buffer*/	
		data_add++;
		i++;
	}
	CB_dump(CircBufferPtr);
printf("Count1:%d\n", CircBufferPtr->count);
	i = 0;
	while(i<3)
	{
		CB_buffer_remove_item(CircBufferPtr, &temp_data);			/*Removes items from the buffer*/
		CB_dump(CircBufferPtr);
		i++;
	}
printf("Count2:%d\n", CircBufferPtr->count);
i=0;
	while(i<2)
	{
		CB_buffer_add_item(CircBufferPtr, data_add);		/*Adding items to the buffer*/	
		data_add++;
		i++;
	}
printf("Count3:%d\n", CircBufferPtr->count);
CB_dump(CircBufferPtr);
CB_buffer_remove_item(CircBufferPtr, &temp_data);
CB_buffer_remove_item(CircBufferPtr, &temp_data);

CB_buffer_remove_item(CircBufferPtr, &temp_data);
CB_buffer_remove_item(CircBufferPtr, &temp_data);
printf("Count4:%d\n", CircBufferPtr->count);
	

}
