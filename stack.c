 /*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*
 *	Title			: Stack Data Structure WS								*	
 *	Author			: Maria Vinokur									   	    *	
 *	Description  	: A header file containing declarations for the funcs	*
 *					  used in impl of Stack data structure					*
 *																			*
 *																			*
 *	HRD30	Infinity Labs R&D		   										*
 *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */

#include "stack.h"
#include <stdlib.h>	/*  size_t, malloc, free*/
#include <stdio.h>  /*  puts   */
#include <assert.h> /*  assert */

#define NUM_OF_BYTES(capacity, size) (capacity) * (size)

struct stk
{
	size_t capacity;
	size_t top;
	size_t element_size;
	void *stack;
};

stack_ty *StackCreate(const size_t capacity, const size_t size_of_element)
{	
	stack_ty *stack = malloc(sizeof(stack_ty));
	if((NULL == stack) || (0 == capacity) || (0 == size_of_element))
	{
		return NULL;
	}
	
	stack -> stack = malloc(capacity * size_of_element);
	if(NULL == stack -> stack)
	{
		free(stack);
		return NULL;
	}
	
	stack -> capacity = capacity;
	stack -> top = 0;
	stack -> element_size = size_of_element;
	
	return (stack);
}

int StackIsEmpty(const stack_ty *stack)
{
	assert(stack);
	
	return (!stack -> top);
}

size_t StackSize(const stack_ty *stack)
{
	assert(stack);
	
	return (stack -> top);
}

size_t StackCapacity(const stack_ty *stack)
{
	assert(stack);
	
	return (stack -> capacity);
}

void *StackPeek(const stack_ty *stack)
{
	size_t current = 0;
	
	unsigned char *ptr_to_stack = NULL;
	
	assert(stack);
	
	if(StackIsEmpty(stack))
	{
		puts("NOTHING TO PEEK, STACK EMPTY");
		return NULL;
	}
	
	ptr_to_stack = (unsigned char *)(stack -> stack);
	
	current = (stack -> top - 1) * stack -> element_size;
	
	return ((void *)(ptr_to_stack + current));
}

void StackDestroy(stack_ty *stack)
{
	assert(stack);
	
	if(stack -> stack != NULL)
	{
		free(stack -> stack);
	}
	
	if(stack != NULL)
	{
		free(stack);
	}
}

void StackPop(stack_ty *stack)
{
	assert(stack);
	
	if(StackIsEmpty(stack))
	{
		return;
	}
	
	--stack -> top;
}

void StackPush(stack_ty *stack, const void *element)
{
	size_t i = 0, current = 0;
	
	unsigned char *ptr_to_el = NULL;
	unsigned char *ptr_to_stack = NULL;
	
	assert(stack);
	assert(element);
	
	ptr_to_el = (unsigned char *)element;
	ptr_to_stack = (unsigned char *)(stack -> stack);
	
	current = stack -> top * stack -> element_size;
	
	if((stack -> top) < (stack -> capacity))
	{
		for(i = 0; i < stack -> element_size; ++i)
		{
			*(ptr_to_stack + current) = *ptr_to_el;
		
			++ptr_to_stack;
			++ptr_to_el;
		}
		
		++stack -> top; 
	}
	
	else
	{
		puts("STACK IS ALREADY FULL");
		return;
	}
}






