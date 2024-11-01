#pragma once
#include "object.h"
#include "vm.h"

/**
 * @brief Create a new integer object within a specific virtual machine context.
 * 
 * @param vm Pointer to the virtual machine context.
 * @param value Integer value to initialize the object with.
 * @return Pointer to the new integer object.
 */
object_t *new_integer_ms(vm_t *vm, int value);

/**
 * @brief Create a new float object within a specific virtual machine context.
 * 
 * @param vm Pointer to the virtual machine context.
 * @param value Float value to initialize the object with.
 * @return Pointer to the new float object.
 */
object_t *new_float_ms(vm_t *vm, float value);

/**
 * @brief Create a new string object within a specific virtual machine context.
 * 
 * @param vm Pointer to the virtual machine context.
 * @param value String value to initialize the object with.
 * @return Pointer to the new string object.
 */
object_t *new_string_ms(vm_t *vm, char *value);

/**
 * @brief Create a new 3D vector object within a specific virtual machine context.
 * 
 * @param vm Pointer to the virtual machine context.
 * @param x X coordinate of the vector.
 * @param y Y coordinate of the vector.
 * @param z Z coordinate of the vector.
 * @return Pointer to the new vector object.
 */
object_t *new_vector3_ms(vm_t *vm, object_t *x, object_t *y, object_t *z);

/**
 * @brief Create a new array object within a specific virtual machine context.
 * 
 * @param vm Pointer to the virtual machine context.
 * @param size Number of elements in the array.
 * @return Pointer to the new array object.
 */
object_t *new_array_ms(vm_t *vm, size_t size);
