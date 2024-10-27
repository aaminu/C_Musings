#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef struct Object object_t;

typedef enum ObjectKind
{
	INTEGER,
	FLOAT,
	STRING,
	VECTOR3,
	ARRAY,
} object_kind_t;

typedef struct Vector
{
	object_t *x;
	object_t *y;
	object_t *z;
} vector_t;

typedef struct Array
{
	size_t size;
	object_t **elements;
} array_t;

typedef union ObjectData
{
	int v_int;
	float v_float;
	char *v_string;
	vector_t v_vector3;
	array_t v_array;
} object_data_t;

typedef struct Object
{
	object_kind_t kind;
	object_data_t data;
	size_t refcount;
	bool is_marked;
} object_t;

object_t *new_integer(int value);
object_t *new_float(float value);
object_t *new_string(char *value);
object_t *new_vector3(object_t *x, object_t *y, object_t *z);
object_t *new_array(size_t size);
bool array_set(object_t *obj, size_t index, object_t *value); 
object_t *array_get(object_t *obj, size_t index); //If Intention is to keep the borrow reference, remember to call to add_reference, and release reference afterwards
int length(object_t *obj);
object_t *add(object_t *a, object_t *b);

void add_reference(object_t *obj); // Mainly for borrow reference
void release_reference(object_t **obj); // to release borrowed reference, can also work on objects, but use obj free preferably
bool object_free(object_t **obj);
