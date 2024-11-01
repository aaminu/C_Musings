#include "stack.h"

stack_t *stack_new(size_t capacity)
{
	stack_t *stack = malloc(sizeof(stack_t));
	if(stack == NULL)
		return NULL;
	
	stack->count = 0;
	stack->capacity = capacity;
	stack->data = malloc(stack->capacity * sizeof(void *));
	if(stack->data == NULL)
	{
		free(stack);
		return NULL;
	}
	
	return stack;
}

void stack_push(stack_t *stack, void *obj)
{
	if (stack->capacity == stack-> count)
	{
		stack->capacity *= 2;
		void **tmptr = realloc(stack->data, stack->capacity * sizeof(void *));
		if (tmptr == NULL)
		{
			free(stack->data);
			exit(1);
		}
		stack->data = tmptr;
	}
	
	stack->data[stack->count] = obj;
	stack->count++;
	return;
}

void *stack_pop(stack_t *stack) 
{
	if (stack->count == 0)
		return NULL;
	
	stack->count--;
	return stack->data[stack->count];
	
}

void stack_free(stack_t *stack) {
  if (stack == NULL || stack->data == NULL)
	  return;
  
  free(stack->data);
  free(stack);
}
