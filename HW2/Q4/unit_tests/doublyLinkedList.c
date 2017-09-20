/**
 * @file
 *		doublyLinkedList.c
 * @brief
 *		This file contains function definitions for doubly linked list implementation
 *
 *
 *		The function definitions in this file are to add nodes to the doubly linked list, remove nodes from doubly linked list, searches for a node in the doubly linked list, returns *      the number of nodes in the list, and destroy the doubly linked list
 * @author
 *		Saritha Senguttuvan
 * @date
 *		Sept 8,2017
 *
 * NOTE: The index starts from 1
 */

#include "doublyLinkedList.h"

LIST_status linked_list_add_node(node_t ** head_ptr, uint32_t data, size_t index)
{
	node_t * item = (node_t *) malloc(sizeof(node_t));			/* allocating space on heap for a node */
	node_t * current_ptr;
	item->data_field = data;
	int8_t i = 0;
	current_ptr = *head_ptr;
	
	if((index > linked_list_size(*head_ptr)+1)	|| (index == 0))	/* check if the index is valid */
		return LIST_ERROR;
	else
	{
		if(index == 1)			/* if first node */
		{
			item->next = *head_ptr;
			*head_ptr = item;		/* making the node as head pointer */
			return LIST_NO_ERR;
		}
		else 
		{
			for(i=0; i<index-2; i++)
				current_ptr = current_ptr->next;
			if(index == linked_list_size(*head_ptr) + 1)		/* if last element */
			{
				current_ptr->next = item;
				item->prev = current_ptr;
				item->next = NULL;
				return LIST_NO_ERR;
			}
			item->next = current_ptr->next;				/* if node is in the middle of linked list */
			current_ptr->next->prev = item;
			current_ptr->next = item;
			return LIST_NO_ERR;
		}
	}
}

LIST_status linked_list_remove_node(node_t ** head_ptr, node_t * temp_node, size_t index)
{
	printf("Remove \n");
	uint8_t i = 0;
	node_t *current_ptr = NULL;
	if(*head_ptr == NULL)
		return LIST_EMPTY;
	current_ptr = *head_ptr;
	
	if((index > linked_list_size(*head_ptr)) || (index == 0))			/* check if the index is valid */
		return LIST_ERROR;	
	else
	{
		if(index == 1) 										/* remove first element */
		{
			temp_node = *head_ptr;
			if((linked_list_size(*head_ptr) == 1))			/* if only one element */
			{
				*head_ptr = NULL;
				return LIST_NO_ERR;
			}
			else
			{
				*head_ptr = (*head_ptr)->next;
				(*head_ptr)->prev = NULL;
			}
			free(temp_node);	
			return LIST_NO_ERR;
		}
		
		for(i=0;i<index-2;i++)
		{	
			current_ptr = current_ptr->next;				
		}	
		temp_node = current_ptr->next;
		if(index == linked_list_size(*head_ptr))					/* if last element */
		{
			current_ptr->next = NULL;
		}
		else												/* if node is in the middle of the linked list */
		{
			current_ptr->next->next->prev = current_ptr;
			current_ptr->next = current_ptr->next->next;
		}
		free(temp_node);
	}
	return LIST_NO_ERR;		
}

LIST_status linked_list_destroy(node_t ** head_ptr)
{
	node_t *temp_node = NULL;
	if(*head_ptr == NULL)
		return LIST_EMPTY;
	while((*head_ptr)->next != NULL)				/* check if last element */
	{
		temp_node = *head_ptr;
		*head_ptr = (*head_ptr)->next;
		free(temp_node);							/* free the memory allocated to the nodes */
	}
	*head_ptr = NULL;							
	free(*head_ptr);								/* free the memory allocated to the last element */
	//printf("Destroyed");
	return LIST_NO_ERR;
}

LIST_status linked_list_search(size_t * index_ptr, node_t * head_ptr, uint32_t data_value)
{
	node_t *current_ptr = NULL;
	size_t index = 1;
	if(head_ptr == NULL)
		return LIST_EMPTY;	
	current_ptr = head_ptr;

	while(current_ptr != NULL)						/* check if node is not NULL */
	{
		if(current_ptr->data_field == data_value)	/* compared the value in the data field and the value to be searched */
		{
			*index_ptr = index;
			return LIST_NO_ERR;
		}
		else
		{
			current_ptr = current_ptr->next;		/* move to the next node */
			index++;
		}
	}
	return LIST_NOT_FOUND;	
}

size_t linked_list_size(node_t * head_ptr)
{
	size_t count = 0;
	node_t *current_ptr = NULL;
	current_ptr = head_ptr;
	if(current_ptr == NULL)
		return LIST_EMPTY;	
	while(current_ptr != NULL)
	{
		current_ptr = current_ptr->next;		/* move to the next node */
		count++;								/* increment the count value */
	}
	return count;
}

void display(node_t * head_ptr)
{
	if(head_ptr == NULL)
		printf("The list is empty\n");
	node_t * current_ptr = head_ptr;
	size_t idx = 1;
	while(current_ptr != NULL)
	{
		printf("Data %ld is %d \n", idx, current_ptr->data_field);		/* print the data and the index */
		idx++;
		current_ptr = current_ptr->next;			/* move to the next node */
	}
}





