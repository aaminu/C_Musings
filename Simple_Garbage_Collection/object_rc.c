#include <stdlib.h>
#include <string.h>
#include "object_rc.h"

/**
 * @brief Create a new object with an initial reference count of 1.
 * 
 * @return Pointer to the newly allocated object, or NULL if allocation fails.
 * 
 * @note This function allocates memory for a new object and initializes its
 *       reference count to 1. It is a static function meant for internal use.
 */
static object_t *_new_object(void)
{
    object_t *ptr = calloc(1, sizeof(object_t));
    if (ptr == NULL)
        return NULL;

    ptr->refcount = 1;
    return ptr;
}

void add_reference(object_t *obj)
{
    if (obj == NULL)
        return;

    obj->refcount++;
}

void release_reference(object_t **obj)
{
    if (obj == NULL || *obj == NULL)
        return;

    (*obj)->refcount--;
    if ((*obj)->refcount == 0)
    {
        object_free(obj);
    }
}

object_t *new_integer(int value)
{
    object_t *ptr = _new_object();
    if (ptr == NULL)
        return NULL;

    ptr->kind = INTEGER;
    ptr->data.v_int = value;

    return ptr;
}

object_t *new_float(float value)
{
    object_t *ptr = _new_object();
    if (ptr == NULL)
        return NULL;

    ptr->kind = FLOAT;
    ptr->data.v_float = value;

    return ptr;
}

object_t *new_string(char *value)
{
    object_t *ptr = _new_object();
    if (ptr == NULL)
        return NULL;

    char *dst = malloc(strlen(value) + 1);
    if (dst == NULL)
    {
        free(ptr);
        return NULL;
    }
    strcpy(dst, value);

    ptr->kind = STRING;
    ptr->data.v_string = dst;

    return ptr;
}

object_t *new_vector3(object_t *x, object_t *y, object_t *z)
{

    if (x == NULL || y == NULL || z == NULL)
        return NULL;

    object_t *ptr = _new_object();
    if (ptr == NULL)
    {
        return NULL;
    }

    ptr->data.v_vector3.x = x;
    add_reference(x);
    ptr->data.v_vector3.y = y;
    add_reference(y);
    ptr->data.v_vector3.z = z;
    add_reference(z);
    ptr->kind = VECTOR3;

    return ptr;
}

object_t *new_array(size_t size)
{
    object_t *ptr = _new_object();
    if (ptr == NULL)
    {
        return NULL;
    }

    object_t **elem_ptr = calloc(size, sizeof(object_t *));
    if (elem_ptr == NULL)
    {
        free(ptr);
        return NULL;
    }

    ptr->kind = ARRAY;
    ptr->data.v_array.size = size;
    ptr->data.v_array.elements = elem_ptr;

    return ptr;
}

bool array_set(object_t *array, size_t index, object_t *value)
{
    if (array == NULL || value == NULL || array->kind != ARRAY)
    {
        return false;
    }

    if (index >= array->data.v_array.size)
    {
        return false;
    }
    
    // Object already at index
    if (array->data.v_array.elements[index] != NULL)
    {
        release_reference(&array->data.v_array.elements[index]);
    }
    
    array->data.v_array.elements[index] = value;
    add_reference(array->data.v_array.elements[index]);
    return true;
}

object_t *array_get(object_t *array, size_t index)
{
    if (array == NULL || array->kind != ARRAY)
    {
        return NULL;
    }

    if (index >= array->data.v_array.size)
    {
        return NULL;
    }
    
    return array->data.v_array.elements[index];
}

int length(object_t *obj)
{
    if (obj == NULL)
    {
        return -1;
    }

    switch (obj->kind)
    {
    case INTEGER:
        return 1;
    case FLOAT:
        return 1;
    case STRING:
        return strlen(obj->data.v_string);
    case VECTOR3:
        return 3;
    case ARRAY:
        return obj->data.v_array.size;
    default:
        return -1;
    }
}

object_t *add(object_t *a, object_t *b)
{
    if (a == NULL || b == NULL)
    {
        return NULL;
    }

    object_t *ptr = NULL;

    switch (a->kind)
    {
    case INTEGER:
        if (b->kind == INTEGER)
        {
            ptr = new_integer(a->data.v_int + b->data.v_int);
        }
        else if (b->kind == FLOAT)
        {
            ptr = new_float(a->data.v_int + b->data.v_float);
        }
        break;

    case FLOAT:
        if (b->kind == INTEGER)
        {
            ptr = new_float(a->data.v_float + b->data.v_int);
        }
        else if (b->kind == FLOAT)
        {
            ptr = new_float(a->data.v_float + b->data.v_float);
        }
        break;

    case STRING:
        if (b->kind != STRING)
        {
            break;
        }

        int len = strlen(a->data.v_string) + strlen(b->data.v_string);
        char *dst = calloc((len + 1), sizeof(char));
        if (dst == NULL)
        {
            break;
        }
        // Transfer strings to temp buffer
        strcpy(dst, a->data.v_string);
        strcat(dst, b->data.v_string);

        ptr = new_string(dst);
        free(dst);

        break;
    case VECTOR3:
        if (b->kind != VECTOR3)
        {
            break;
        }

        object_t *new_x = add(a->data.v_vector3.x, b->data.v_vector3.x);
        object_t *new_y = add(a->data.v_vector3.y, b->data.v_vector3.y);
        object_t *new_z = add(a->data.v_vector3.z, b->data.v_vector3.z);
        if (new_x == NULL || new_y == NULL || new_z == NULL)
        { // If any  fails
            if (new_x)
                release_reference(&new_x);
            if (new_y)
                release_reference(&new_y);
            if (new_z)
                release_reference(&new_z);
            break;
        }

        ptr = new_vector3(new_x, new_y, new_z);
        // Release initial reference since objects within this scope are not needded
        release_reference(&new_x);
        release_reference(&new_y);
        release_reference(&new_z);
        break;

    case ARRAY:
        if (b->kind != ARRAY)
        {
            break;
        }

        size_t size = a->data.v_array.size + b->data.v_array.size;
        ptr = new_array(size);
        if (ptr == NULL)
        {
            break;
        }

        for (size_t i = 0; i < a->data.v_array.size; i++)
        {
            object_t *obj = array_get(a, i);
            array_set(ptr, i, obj);

        }
        for (size_t i = 0; i < b->data.v_array.size; i++)
        {
            object_t *obj = array_get(b, i);
            array_set(ptr, (i + a->data.v_array.size), obj);
        }
        break;

    default:
        break;
    }

    return ptr;
}

bool object_free(object_t **obj)
{
    if (obj == NULL || *obj == NULL)
        return true;
    
    if ((*obj)->refcount > 1)
     return false;

    switch ((*obj)->kind)
    {
    case INTEGER:
    case FLOAT:
        break;

    case STRING:
        // Free the dynamically allocated string,
        free((*obj)->data.v_string);
        (*obj)->data.v_string = NULL;
        break;

    case VECTOR3:
        // Reduce ref count for vector component, auto free if zero aka object only reference
        release_reference(&(*obj)->data.v_vector3.x);
        release_reference(&(*obj)->data.v_vector3.y);
        release_reference(&(*obj)->data.v_vector3.z);
        break;

    case ARRAY:
        if ((*obj)->data.v_array.elements != NULL)
        {
            for (size_t i = 0; i < (*obj)->data.v_array.size; i++)
            {
                if ((*obj)->data.v_array.elements[i] != NULL)
                {
                    release_reference(&(*obj)->data.v_array.elements[i]); // Remove reference `object_t *`
                }
            }
            free((*obj)->data.v_array.elements);
            (*obj)->data.v_array.elements = NULL;
        }

        break;

    default:
        break;
    }

    free(*obj);
    *obj = NULL;

    return true;
}
