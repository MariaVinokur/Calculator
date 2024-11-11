/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*
 *	Title			: Stack Data Structure WS								*	
 *	Author			: Maria Vinokur									   	*	
 *	Description  	: A header file containing declarations for the funcs	*
 *					  used in impl of Stack data structure					*
 *																			*
 *																			*
 *	HRD30	Infinity Labs R&D		   										*
 *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*/

#include <stdio.h>  /*  printf, puts  */
#include <assert.h> /*  assert  */
#include "stack.h"

#define CAPACITY 5
#define ELEMENT_SIZE 4


static int IsStackCreated(stack_ty *st);
static void SwapPointers(int **ptr1, int **ptr2);

int main(void)
{
	stack_ty *my_stack = NULL;
	
	int num = 5;
	int num2 = 3;
	int num3 = 6;
	
	int a = 5;
	int b = 6;
	
	int *p1 = &a;
	int *p2 = &b;
	
	printf("11a: %d, b: %d\n", a, b);
	printf("11*p1: %d, p1address: %p, p2: %d, p2address: %p\n", *p1, p1, *p2, p2);
	SwapPointers(&p1, &p2);
	
	printf("11a: %d, b: %d\n", a, b);
	

	printf("22*p1: %d, p1address: %p, p2: %d, p2address: %p\n", *p1, p1, *p2, p2);
	my_stack = StackCreate(CAPACITY, ELEMENT_SIZE);
	
	printf("IsStackCreated(): %d\n", IsStackCreated(my_stack));
	
	StackPush(my_stack, &num);
	StackPush(my_stack, &num2);
	StackPush(my_stack, &num3);
	
	StackPeek(my_stack);
	
	if(*(int *)StackPeek(my_stack) == 6)
	{
		puts("PEEK IS WORKING");
	}
	
	StackPop(my_stack);
	
	if(*(int *)StackPeek(my_stack) == 3)
	{
		puts("POP IS WORKING");
	}
	
	if(StackSize(my_stack) == 2)
	{
		puts("SIZE IS WORKING");
	}
	
	if(StackCapacity(my_stack) == CAPACITY)
	{
		puts("CAPACITY IS WORKING");
	}
	
	StackPush(my_stack, &num3);
	
	if(*(int *)StackPeek(my_stack) == 3)
	{
		puts("PUSH IS WORKING");
	}

	
	StackPop(my_stack);
	StackPop(my_stack);
	StackPop(my_stack);
	
	if(StackIsEmpty(my_stack) == 1)
	{
		puts("EMPTY IS WORKING");
	}
	
	
	
	/*StackDestroy(my_stack);
	StackPeek(my_stack);*/
	
	return 0;
}

static void SwapPointers(int **ptr1, int **ptr2)
{

	int *temp = *ptr1;
	printf("ptr1: %p, ptr2: %p\n", ptr1, ptr2);
	*ptr1 = *ptr2;
	*ptr2 = temp;

}


static int IsStackCreated(stack_ty *st)
{
	assert(st);
	
	return !!st;
}

