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

#include "circbuf.h"

void test_cb_add(void **state)
{
	CB_t * ptr;
	uint32_t data1 = 15;
	uint32_t data2 = 20;
	size_t len = 7;
	assert_int_equal(CB_init(&ptr,len),BUFF_NO_ERR);

	assert_int_equal(CB_buffer_add_item(ptr, data1),BUFF_NO_ERR);		/*Checks if item added successfully*/
	assert_int_equal(CB_buffer_add_item(ptr, data2),BUFF_NO_ERR);		/*Checks if item added successfully*/	
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/
}

void test_cb_remove(void **state)
{
	CB_t * ptr;
	uint32_t data1 = 15;
	uint32_t data2 = 20;
	uint32_t temp_data = 0;
	assert_int_equal(CB_init(&ptr,7),BUFF_NO_ERR);

	assert_int_equal(CB_buffer_add_item(ptr, data1),BUFF_NO_ERR);		/*Checks if item added successfully*/
	assert_int_equal(CB_buffer_add_item(ptr, data2),BUFF_NO_ERR);		/*Checks if item added successfully*/
	
	assert_int_equal(CB_buffer_remove_item(ptr,&temp_data),BUFF_NO_ERR);	/*Checks if item is removed successfully*/
	assert_int_equal(temp_data,15);						/*validates the data*/
	assert_int_equal(CB_buffer_remove_item(ptr,&temp_data),BUFF_NO_ERR);	/*Checks if item is removed successfully*/
	assert_int_equal(temp_data,20);						/*validates the data*/

	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/
}

void test_cb_full(void **state)
{
	CB_t * ptr;				/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;						/*total number of elements possible*/
	assert_int_equal(CB_init(&ptr,no_of_items),BUFF_NO_ERR);			/*initializes the buffer*/
	uint8_t i = 0;
	uint32_t data = 5;
	while(i<no_of_items)
	{
		assert_int_equal(CB_buffer_add_item(ptr, data),BUFF_NO_ERR);	/*Checks if item added successfully*/
		data++;
		i++;
	}
	assert_int_equal(CB_is_full(ptr),BUFF_FULL);				/*checks if the buffer is full*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/
}

void test_cb_empty(void **state)
{
	CB_t * ptr;				/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;						/*total number of elements possible*/
	assert_int_equal(CB_init(&ptr,no_of_items),BUFF_NO_ERR);			/*initializes the buffer*/	
	assert_int_equal(CB_is_empty(ptr),BUFF_EMPTY);				/*checks if the buffer is empty*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/	
}

void test_cb_alloc(void **state)
{
	CB_t * ptr;
	assert_int_equal(CB_init(&ptr,7),BUFF_NO_ERR);				/*checks for no failure*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure*/
}

void test_cb_free(void **state)
{
	CB_t * ptr;
	assert_int_equal(CB_init(&ptr,7),BUFF_NO_ERR);				/*checks for no failure*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure*/
	assert_null((void*) ptr->buff_ptr);					/*Checks if the ptr is pointing to NULL*/
}

void test_cb_buff_null(void **state)
{
	CB_t * ptr = NULL;
	assert_int_equal(CB_buffer_add_item(ptr, 7),BUFF_NULL);			/*Checks if the ptr is pointing to NULL*/
	//ptr = (CB_t*)malloc(sizeof(CB_t));
	assert_int_equal(CB_init(&ptr,7),BUFF_NO_ERR);				/*checks for no failure*/
	assert_int_not_equal(CB_buffer_add_item(ptr, 7),BUFF_NULL);		/*Checks if the ptr is pointing to NULL*/            
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);			        /*checks for no failure*/
}

void test_cb_init(void **state)
{
	CB_t * ptr;				/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;
	assert_int_equal(CB_init(&ptr,no_of_items), BUFF_NO_ERR); 
	assert_int_equal(ptr->buff_ptr,ptr->head_ptr);			
	assert_int_equal(ptr->buff_ptr,ptr->tail_ptr);	/*checks is buffer pointer and tail pointer are pointing to the same location*/
	assert_int_equal(ptr->total_length, no_of_items);  /*checks if total possible length is equal to number of items initialized using CB_init*/
	assert_int_equal(ptr->count,0);						/*checks if total number of items in the buffer is 0*/
	assert_non_null((void*)ptr->buff_ptr);					/*checks if the buffer pointer is not pointing to NULL*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/
}

void test_cb_wrap_add(void **state)
{	
	CB_t * ptr;				/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;						/*total number of elements possible*/
	assert_int_equal(CB_init(&ptr,no_of_items),BUFF_NO_ERR);			/*initializes the buffer*/
	uint8_t i = 0;
	uint32_t data = 5;
	while(i<no_of_items)
	{
		assert_int_equal(CB_buffer_add_item(ptr, data),BUFF_NO_ERR);	/*Checks if item added successfully*/
		data++;
		i++;
	}
	assert_int_equal(CB_is_full(ptr),BUFF_FULL);			/*checks if the buffer is full*/
	assert_ptr_equal((void*)ptr->head_ptr, (void*)ptr->tail_ptr); 	/*checks if head and tail are pointing to the same location*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);			/*checks for no failure after destroying*/		
}

void test_cb_wrap_remove(void **state)
{
	CB_t * ptr;			/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;					/*total number of elements possible*/
	assert_int_equal(CB_init(&ptr,no_of_items),BUFF_NO_ERR);		/*initializes the buffer*/
	uint8_t i = 0;
	uint32_t data = 5;
	uint32_t temp_data = 0;
	while(i<no_of_items)
	{
		assert_int_equal(CB_buffer_add_item(ptr, data),BUFF_NO_ERR);	/*Checks if item added successfully*/
		data++;
		i++;
	}
	i = 0;
	while(i<no_of_items)
	{
		assert_int_equal(CB_buffer_remove_item(ptr, &temp_data),BUFF_NO_ERR);	/*Checks if item added successfully*/
		i++;
	}
	assert_int_equal(CB_is_empty(ptr),BUFF_EMPTY);				/*checks if the buffer is empty*/
	assert_ptr_equal((void*)ptr->head_ptr, (void*)ptr->tail_ptr);		/*checks if head and tail are pointing to the same location*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/		
}

void test_cb_over_fill(void **state)
{	
	CB_t * ptr;				/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;						/*total number of elements possible*/
	assert_int_equal(CB_init(&ptr,no_of_items),BUFF_NO_ERR);			/*initializes the buffer*/
	uint8_t i = 0;
	uint32_t data = 5;
	while(i<no_of_items)
	{
		assert_int_equal(CB_buffer_add_item(ptr, data),BUFF_NO_ERR);	/*Checks if item added successfully*/
		data++;
		i++;
	}
	assert_int_equal(CB_buffer_add_item(ptr, data),BUFF_ERROR);		/*checks if the buffer is full*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/
}

void test_cb_over_empty(void **state)
{	
	CB_t * ptr = (CB_t*)malloc(sizeof(CB_t));				/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;						/*total number of elements possible*/
	assert_int_equal(CB_init(&ptr,no_of_items),BUFF_NO_ERR);			/*initializes the buffer*/
	uint8_t i = 0;
	uint32_t data = 5;
	uint32_t temp_data = 0;
	while(i<no_of_items)
	{
		//printf("Tha add value of i %d returned is %d\n",i, CB_buffer_add_item(ptr, data));
		assert_int_equal(CB_buffer_add_item(ptr, data),BUFF_NO_ERR);	/*Checks if item added successfully*/
		data++;
		i++;
	}
	i = 0;
	while(i<no_of_items)
	{
		//printf("Tha value of i %d returned is %d\n",i, CB_buffer_remove_item(ptr, &temp_data));
		assert_int_equal(CB_buffer_remove_item(ptr, &temp_data),BUFF_NO_ERR);	/*Checks if removed added successfully*/
		i++;
	}
	
	assert_int_equal(CB_buffer_remove_item(ptr, &temp_data),BUFF_ERROR);			/*Checks if Buffer is empty*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);						/*checks for no failure after destroying*/
}

void test_cb_size(void **state)
{	
	CB_t * ptr;				/*allocates memory for the buffer*/	
	uint8_t no_of_items = 4;						/*total number of elements possible*/
	assert_int_equal(CB_init(&ptr,no_of_items),BUFF_NO_ERR);			/*initializes the buffer*/
	uint8_t i = 0;
	uint32_t data = 5;
	while(i<no_of_items)
	{
		assert_int_equal(CB_buffer_add_item(ptr, data),BUFF_NO_ERR);	/*Checks if item added successfully*/
		data++;
		i++;
	}
	assert_int_equal(CB_size(ptr),4);		/*checks if the buffer has 4 elements*/
	assert_int_equal(CB_destroy(ptr),BUFF_NO_ERR);				/*checks for no failure after destroying*/
}

int main(int argc, char **argv)
{
	const struct CMUnitTest tests[] = {	
	/*circular buffer unit tests*/
	cmocka_unit_test(test_cb_alloc),
	cmocka_unit_test(test_cb_free),	
	cmocka_unit_test(test_cb_buff_null),
	cmocka_unit_test(test_cb_init),	
	cmocka_unit_test(test_cb_add),
	cmocka_unit_test(test_cb_remove),
    cmocka_unit_test(test_cb_full),
	cmocka_unit_test(test_cb_empty),
	cmocka_unit_test(test_cb_wrap_add),
	cmocka_unit_test(test_cb_wrap_remove),
	cmocka_unit_test(test_cb_over_fill),
	cmocka_unit_test(test_cb_over_empty),
	cmocka_unit_test(test_cb_size),
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
