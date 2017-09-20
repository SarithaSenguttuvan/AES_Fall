/**
 * @file 
 *		main.c 
 * @brief 
 *		In this file a call to the main function is made which in turn makes function calls to test Doubly linked list functioanalities
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
#include "doublyLinkedList.h"

#define SEARCH_DATA    15

node_t *head_ptr = NULL;				/* points to the head node of the doubly linked list */
node_t *tmp = NULL;

int main()
{
	LIST_status dll_status;
	uint32_t add_Data = 10;
	size_t idx = 1;
	size_t idx_ptr = 0;
	size_t total_nodes = 0;
	dll_status = linked_list_add_node(&head_ptr, add_Data, idx);		/* add node the linked list */
	dll_status = linked_list_add_node(&head_ptr, 12, 2);			/* add node the linked list */
	dll_status = linked_list_add_node(&head_ptr, 15, 3);			/* add node the linked list */
	dll_status = linked_list_add_node(&head_ptr, 17, 4);			/* add node the linked list */
	dll_status = linked_list_add_node(&head_ptr, 21, 5);			/* add node the linked list */
	display(head_ptr);
	dll_status = linked_list_remove_node(&head_ptr, tmp, 5);		/* remove the node with index 5 */
	display(head_ptr);
	total_nodes = linked_list_size(head_ptr);				/* function call to determine the size of the linked list */
	printf("The total number of nodes are %ld\n", total_nodes);	
	linked_list_search(&idx_ptr, head_ptr,SEARCH_DATA);		/* function call to find a node with data field*/
	printf("The element was found at index %ld \n", idx_ptr);
	linked_list_destroy(&head_ptr);					/* function call to destroy the linked list */
	display(head_ptr);
}
