#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef struct Object object_t;

/**
 * @enum ObjectKind
 * Enum to define types of objects.
 */
typedef enum ObjectKind {
    INTEGER,   /**< Integer type */
    FLOAT,     /**< Float type */
    STRING,    /**< String type */
    VECTOR3,   /**< 3D vector type */
    ARRAY      /**< Array type */
} object_kind_t;

/**
 * @struct Vector
 * Structure to represent a 3D vector.
 */
typedef struct Vector {
    object_t *x; /**< X coordinate */
    object_t *y; /**< Y coordinate */
    object_t *z; /**< Z coordinate */
} vector_t;

/**
 * @struct Array
 * Structure to represent an array of objects.
 */
typedef struct Array {
    size_t size;           /**< Size of the array */
    object_t **elements;   /**< Pointer to array elements */
} array_t;

/**
 * @union ObjectData
 * Union to hold data for different object types.
 */
typedef union ObjectData {
    int v_int;             /**< Integer value */
    float v_float;         /**< Float value */
    char *v_string;        /**< String value */
    vector_t v_vector3;    /**< 3D vector */
    array_t v_array;       /**< Array of objects */
} object_data_t;

/**
 * @struct Object
 * Structure to represent a generic object with a type and data.
 */
typedef struct Object {
    object_kind_t kind;    /**< Kind of the object */
    object_data_t data;    /**< Data of the object */
    size_t refcount;       /**< Reference count */
    bool is_marked;        /**< Mark for garbage collection */
} object_t;
