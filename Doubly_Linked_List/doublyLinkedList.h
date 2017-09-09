/**
 * @file
 *		doublyLinkedList.h
 * @brief
 *		This file contains function declarations for doubly linked list implementation
 *
 *
 *		The function declarations in this file are to add nodes to the doubly linked list, remove nodes from doubly linked list, searches for a node in the doubly linked list, returns *      the number of nodes in the list, and destroy the doubly linked list
 * @author
 *		Saritha Senguttuvan
 * @date
 *		Sept 8,2017
 *
 */

#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

/* structure of a node */
typedef struct node_t{
   uint32_t data_field;					/* Data field which holds the data in a node */	
   struct node_t *next;
   struct node_t *prev;
}node_t;

/* Enum for representing thr return values */
typedef enum{
	LIST_NOT_FOUND,					/*enum when buffer is Full*/
	LIST_EMPTY,						/*enum when buffer is Empty*/
	LIST_NO_ERR,					/*enum when there is no error*/
	LIST_ERROR,						/*enum there is error*/
	LIST_NULL						/*enum when the pointer is pointing to NULL*/
}LIST_status;

//struct node_t *head_ptr = NULL;				/* points to the head node of the doubly linked list */
//struct node_t *tail_ptr = NULL;				/* points to the last node of the doubly linked list */
//struct node_t *current_ptr = NULL;				/* current node in the linked list */

/**
 * @brief function to add items to the doubly linked list
 *
 * This function implements functionality of adding items to doubly linked list pointed to by head_ptr
 * data holds the item value that needs to be added in the data field
 *
 * @return CB_status (enum)
 */
LIST_status linked_list_add_node(node_t ** head_ptr, uint32_t data, size_t index);

/**
 * @brief function to remove items from the doubly linked list
 *
 * This function implements functionality of removing items from doubly linked list pointed to by base_ptr
 * data holds the item value that needs to be added in the data field
 *
 * @return CB_status (enum)
 */
LIST_status linked_list_remove_node(node_t ** head_ptr, node_t * temp_node, size_t index);

/**
 * @brief function to destroy all the nodes in the doubly linked list
 *
 * This function implements functionality of destroying all the  nodes
 *
 * @return LIST_status (enum)
 */
LIST_status linked_list_destroy(node_t ** head_ptr);

/**
 * @brief function to search for data at a given node 
 *
 * This function implements functionality of searching the node that holds the desired data
 *
 * @return node_t - returns pointer to the search element 
 */
LIST_status linked_list_search(size_t * index_ptr, node_t * head_ptr, uint32_t data_value);

/**
 * @brief function to determine the number of nodes in the doubly linked list
 *
 * This function implements functionality of obtaining the number of elements in the doubly list
 *
 * @return LIST_status (enum)
 */
size_t linked_list_size(node_t * head_ptr);



void display(node_t * head_ptr);

#endif



















