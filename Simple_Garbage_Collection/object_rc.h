#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "object.h"


/**
 * @brief Create a new integer object.
 * 
 * @param value Integer value to initialize the object with.
 * @return Pointer to the new integer object.
 */
object_t *new_integer(int value);

/**
 * @brief Create a new float object.
 * 
 * @param value Float value to initialize the object with.
 * @return Pointer to the new float object.
 */
object_t *new_float(float value);

/**
 * @brief Create a new string object.
 * 
 * @param value String value to initialize the object with.
 * @return Pointer to the new string object.
 */
object_t *new_string(char *value);

/**
 * @brief Create a new 3D vector object.
 * 
 * @param x X coordinate of the vector.
 * @param y Y coordinate of the vector.
 * @param z Z coordinate of the vector.
 * @return Pointer to the new vector object.
 */
object_t *new_vector3(object_t *x, object_t *y, object_t *z);

/**
 * @brief Create a new array object.
 * 
 * @param size Number of elements in the array.
 * @return Pointer to the new array object.
 */
object_t *new_array(size_t size);

/**
 * @brief Set an element in an array object.
 * 
 * @param obj Array object.
 * @param index Index at which to set the value.
 * @param value Value to set at the specified index.
 * @return True if successful, false otherwise.
 */
bool array_set(object_t *obj, size_t index, object_t *value);

/**
 * @brief Get an element from an array object.
 * 
 * @param obj Array object.
 * @param index Index from which to get the value.
 * @return Pointer to the object at the specified index.
 * 
 * @note If the intention is to keep the borrowed reference, remember to call 
 * `add_reference` and release the reference by `release_reference`afterwards.
 */
object_t *array_get(object_t *obj, size_t index);

/**
 * @brief Get the length of an object.
 * 
 * @param obj Object to get the length of.
 * @return Length of the object.
 */
int length(object_t *obj);

/**
 * @brief Add two objects.
 * 
 * @param a First object.
 * @param b Second object.
 * @return Pointer to the result of the addition.
 */
object_t *add(object_t *a, object_t *b);

/**
 * @brief Increase the reference count of an object.
 * 
 * @param obj Object to add a reference to.
 * 
 * @note Mainly to add reference to a borrowed object
 */
void add_reference(object_t *obj);

/**
 * @brief Release the reference to an object.
 * 
 * @param obj Pointer to the object to release.
 * 
 * @note Release reference to a borrowed object
 */
void release_reference(object_t **obj);

/**
 * @brief Free an object and release its resources.
 * 
 * @param obj Pointer to the object to free.
 * @return True if the object was freed, false otherwise.
 */
bool object_free(object_t **obj);
