/**
 * @file
 *		circbuf.h
 * @brief
 *		This file contains function declarations for circular buffer implementation
 *
 *
 *		The function declarations in this file are to add items to the circular buffer, delete items from circular buffer, check if the circular buffer is full/empty
 *      allocate memory for the circular buffer, and destroy the circular buffer
 * @author
 *		Saritha Senguttuvan
 * @date
 *		Sept 8,2017
 *
 */

#ifndef CIRC_BUFF_H
#define CIRC_BUFF_H

#include <stdint.h>

typedef struct{
	uint32_t * buff_ptr;
	uint32_t * head_ptr;			/*Pointer to the newly added itme*/
	uint32_t * tail_ptr;			/*Pointer to the oldest item*/
	size_t total_length;			/*Maximum number of items allowed*/
	size_t count;					/*Number of items in the buffer*/
}CB_t;

typedef enum{
	BUFF_FULL,						/*enum when buffer is Full*/
	BUFF_EMPTY,						/*enum when buffer is Empty*/
	BUFF_NO_ERR,					/*enum when there is no error*/
	BUFF_ERROR,						/*enum there is error*/
	BUFF_NULL						/*enum when the pointer is pointing to NULL*/
}CB_status;

/**
 * @brief function to add items to the buffer
 *
 * This function implements functionality of add items to buffer pointed to by buff_ptr
 * data holds the item value that needs to be added
 *
 * @return CB_status (enum)
 */
CB_status CB_buffer_add_item(CB_t * CB_buff_ptr, uint32_t data);

/**
 * @brief function to remove items from the buffer
 *
 * This function implements functionality of remove items from the buffer pointed to by buff_ptr
 * variable data, stores and returns the item value that needs to be removed
 *
 * @return CB_status (enum)
 */
CB_status CB_buffer_remove_item(CB_t * CB_buff_ptr, uint32_t *temp_data);

/**
 * @brief function checks if the buffer is full
 *
 * This function implements functionality of checking if the number of data items is equal to the
 * maximum length allowed
 *
 * @return CB_status (enum)
 */
CB_status CB_is_full(CB_t * CB_buff_ptr);

/**
 * @brief function checks if the buffer is empty
 *
 * This function implements functionality of checking if there are no data elements in the buffer
 *
 * @return CB_status (enum)
 */
CB_status CB_is_empty(CB_t * CB_buff_ptr);

/**
 * @brief function to allocate memory for buffer
 *
 * This function implements functionality of allocating memory to buffer, by specifying the size
 *
 * @return CB_status (enum)
 */
CB_status CB_init(CB_t ** CB_buff_ptr, size_t len);

/**
 * @brief function to point to the circular buffer to be destroyed
 *
 * This function implements functionality of destroying the buffer
 *
 * @return CB_status (enum)
 */
CB_status CB_destroy(CB_t * CB_buff_ptr);

/**
 * @brief function to point to the circular buffer to be printed
 *
 * This function implements functionality of displaying the contents of the buffer
 *
 * @return CB_status (enum)
 */
CB_status CB_dump(CB_t * CB_buff_ptr);
/**
 * @brief function to determine the size of the circular buffer
 *
 * This function implements functionality of displaying the size of the buffer
 *
 * @return CB_status (enum)
 */
size_t CB_size(CB_t * CB_buff_ptr);
#endif
