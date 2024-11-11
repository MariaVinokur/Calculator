/*  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*
 *	Title			: Stack Data Structure WS								*	
 *	Author			: Maria Vinokur									   	*	
 *	Description  	: A header file containing declarations for the funcs	*
 *					  used in impl of Stack data structure					*
 *																			*
 *																			*
 *	HRD30	Infinity Labs R&D		   										*
 *  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 	*/

#ifndef STACK_H
#define STACK_H

#include <stddef.h>   /*   size_t  */

typedef struct stk stack_ty;

/*	creates stack, capacity * size_of_element in size 			O(1)		*/
stack_ty *StackCreate(const size_t capacity, const size_t size_of_element);

/*	clears stack												O(1)		*/
void StackDestroy(stack_ty *stack);

/*	removes topmost member from stack							O(1)		*/
void StackPop(stack_ty *stack);

/*	pushes new member onto stack								O(1)		*/
void StackPush(stack_ty *stack, const void *element);

/*	returns top member of stack									O(1)		*/
void *StackPeek(const stack_ty *stack);

/*	returns size of stack (how many members were pushed)		O(1)		*/
size_t StackSize(const stack_ty *stack);

/*	returns TRUE/FALSE if stack empty/not empty					O(1)		*/
/*	by C89 convention, boolean return vals are defined as int				*/
int StackIsEmpty(const stack_ty *stack);

/*	returns max capacity of stack								O(1)		*/
size_t StackCapacity(const stack_ty *stack);


#endif					/*	include guard	*/
