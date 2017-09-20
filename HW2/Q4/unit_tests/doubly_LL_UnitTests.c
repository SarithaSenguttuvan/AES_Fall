/**
 * @file 
 *		circBuf_UnitTests.c
 * @brief 
 *		This file contains unit tests for circular buffer
 *
 * @author 
 *		Saritha Senguttuvan
 * @date 
 *		Sept,2017
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>

#include "doublyLinkedList.h"

void test_cb_add_node(void **state)
{
	node_t *head_ptr = NULL;
	uint32_t add_Data = 10;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(head_ptr->data_field,10);
	assert_int_equal(linked_list_add_node(&head_ptr, 12, 2), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);		/* destroy the linked list */
}

void test_cb_add_node_idx_err(void **state)
{
	node_t *head_ptr = NULL;
	uint32_t add_Data = 10;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 3), LIST_ERROR);		/* add node the linked list */
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);		/* destroy the linked list */
}

void test_cb_remove_node(void **state)
{	
    node_t *head_ptr = NULL;
    node_t *tmp = NULL;
	uint32_t add_Data = 10;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 12, 2), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(head_ptr->data_field,10);
	assert_int_equal(linked_list_remove_node(&head_ptr, tmp, 1), LIST_NO_ERR);				/* remove node from the linked list */
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);		/* destroy the linked list */	
}

void test_cb_remove_node_idx_err(void **state)
{	
    node_t *head_ptr = NULL;
    node_t *tmp = NULL;
	uint32_t add_Data = 10;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 12, 2), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_remove_node(&head_ptr, tmp, 4), LIST_ERROR);				/* remove node from the linked list */
	assert_int_equal(linked_list_remove_node(&head_ptr, tmp, 0), LIST_ERROR);				/* remove node from the linked list */
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);		/* destroy the linked list */	
}

void test_cb_remove_node_empty(void **state)
{	
    node_t *head_ptr = NULL;
    node_t *tmp = NULL;
	assert_int_equal(linked_list_remove_node(&head_ptr, tmp, 1), LIST_EMPTY);				/* remove node from the linked list */
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_EMPTY);		/* destroy the linked list */	
}


void test_cb_destroy_node(void **state)
{
	node_t *head_ptr = NULL;
	uint32_t add_Data = 10;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 2), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);		/* destroy the linked list */
}

void test_cb_destroy_node_empty(void **state)
{
	node_t *head_ptr = NULL;
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_EMPTY);		/* destroy the linked list */
}

void test_cb_search_node(void **state)
{
	node_t *head_ptr = NULL;
	uint32_t add_Data = 10;
	size_t idx_ptr = 0;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 12, 2), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 15, 3), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(head_ptr->data_field,10);
	assert_int_equal(linked_list_search(&idx_ptr, head_ptr, 12), LIST_NO_ERR);
	assert_int_equal(idx_ptr,2);
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);	    /* destroy the linked list */
}

void test_cb_search_node_not_found(void **state)
{
	node_t *head_ptr = NULL;
	uint32_t add_Data = 10;
	size_t idx_ptr = 0;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 12, 2), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 15, 3), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_search(&idx_ptr, head_ptr, 100), LIST_NOT_FOUND);
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);		/* destroy the linked list */
}

void test_cb_search_node_empty(void **state)
{
	node_t *head_ptr = NULL;
	size_t idx_ptr = 0;
	assert_int_equal(linked_list_search(&idx_ptr, head_ptr, 100), LIST_EMPTY);
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_EMPTY);		/* destroy the linked list */
}

void test_cb_size(void **state)
{
	node_t *head_ptr = NULL;
	uint32_t add_Data = 10;
	assert_int_equal(linked_list_add_node(&head_ptr, add_Data, 1), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 12, 2), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 15, 3), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 12, 4), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_add_node(&head_ptr, 15, 5), LIST_NO_ERR);		/* add node the linked list */
	assert_int_equal(linked_list_size(head_ptr), 5);
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_NO_ERR);		/* destroy the linked list */
}

void test_cb_size_empty(void **state)
{
	node_t *head_ptr = NULL;
	assert_int_equal(linked_list_size(head_ptr), LIST_EMPTY);
	assert_int_equal(linked_list_destroy(&head_ptr), LIST_EMPTY);		/* destroy the linked list */
}

int main(int argc, char **argv)
{
	const struct CMUnitTest tests[] = {	
	/*Doubly Linked List unit tests*/
    cmocka_unit_test(test_cb_add_node),
    cmocka_unit_test(test_cb_add_node_idx_err),
    cmocka_unit_test(test_cb_remove_node),
    cmocka_unit_test(test_cb_remove_node_idx_err),
    cmocka_unit_test(test_cb_remove_node_empty),
    cmocka_unit_test(test_cb_destroy_node),
    cmocka_unit_test(test_cb_destroy_node_empty),
    cmocka_unit_test(test_cb_search_node),
    cmocka_unit_test(test_cb_search_node_not_found),
    cmocka_unit_test(test_cb_search_node_empty),
    cmocka_unit_test(test_cb_size),
    cmocka_unit_test(test_cb_size_empty),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
