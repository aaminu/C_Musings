#pragma once

#include <stddef.h>
#include <stdlib.h>

/**
 * @struct Stack
 * @brief A stack structure for managing a dynamic array of generic pointers.
 * 
 * This stack supports dynamic resizing and stores a collection of void pointers,
 * allowing it to hold any type of object.
 */
typedef struct Stack
{
    size_t count;      /**< Current number of elements in the stack */
    size_t capacity;   /**< Maximum capacity of the stack before resizing */
    void **data;       /**< Array of pointers to stack elements */
} stack_t;

/**
 * @brief Create a new stack with a specified initial capacity.
 * 
 * @param capacity Initial capacity of the stack.
 * @return Pointer to the newly created stack, or NULL if allocation fails.
 */
stack_t *stack_new(size_t capacity);

/**
 * @brief Push an object onto the stack.
 * 
 * @param stack Pointer to the stack.
 * @param obj Pointer to the object to push onto the stack.
 * 
 * @note The stack will resize by a factor of 2 automatically if the current capacity is exceeded.
 */
void stack_push(stack_t *stack, void *obj);

/**
 * @brief Pop the top object from the stack.
 * 
 * @param stack Pointer to the stack.
 * @return Pointer to the object at the top of the stack, or NULL if the stack is empty.
 */
void *stack_pop(stack_t *stack);

/**
 * @brief Free the stack and its allocated resources.
 * 
 * @param stack Pointer to the stack to free.
 * 
 * @note This function does not free the objects contained within the stack; it only frees the stack structure itself.
 */
void stack_free(stack_t *stack);
