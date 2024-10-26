#ifndef _STACK_H_
#define _STACK_H_

#include <stddef.h>
#include <stdlib.h>

typedef struct Stack
{
	size_t count;
	size_t capacity;
	void **data;
} stack_t;

stack_t *stack_new(size_t capacity);
void stack_push(stack_t *stack, void *obj);
void *stack_pop(stack_t *stack); 
void stack_free(stack_t *stack);


#endif /*_STACK_H*/
