#include <string.h>

#include "object_ms.h"

/**
 * @brief Create a new object within a specific virtual machine context and track it.
 * 
 * @param vm Pointer to the virtual machine context used to track the object.
 * @return Pointer to the newly allocated object, or NULL if allocation fails.
 * 
 * @note This function allocates memory for a new object, sets its marked state to false, 
 *       and tracks it in the provided virtual machine context. It is a static function meant for internal use.
 */
static object_t *_new_object_tr(vm_t *vm)
{
    object_t *obj = calloc(1, sizeof(object_t));
    if (obj == NULL)
        return NULL;

    obj->is_marked = false;
    vm_track_object(vm, obj);
    return obj;
}

object_t *new_integer_ms(vm_t *vm, int value)
{
    object_t *ptr = _new_object_tr(vm);
    if (ptr == NULL)
        return NULL;

    ptr->kind = INTEGER;
    ptr->data.v_int = value;

    return ptr;
}

object_t *new_float_ms(vm_t *vm, float value)
{
    object_t *ptr = _new_object_tr(vm);
    if (ptr == NULL)
        return NULL;

    ptr->kind = FLOAT;
    ptr->data.v_float = value;

    return ptr;
}

object_t *new_string_ms(vm_t *vm, char *value)
{
    object_t *ptr = _new_object_tr(vm);
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

object_t *new_vector3_ms(vm_t *vm, object_t *x, object_t *y, object_t *z)
{

    if (x == NULL || y == NULL || z == NULL)
        return NULL;

    object_t *ptr = _new_object_tr(vm);
    if (ptr == NULL)
    {
        return NULL;
    }

    ptr->data.v_vector3.x = x;
    ptr->data.v_vector3.y = y;
    ptr->data.v_vector3.z = z;
    ptr->kind = VECTOR3;

    return ptr;
}

object_t *new_array_ms(vm_t *vm, size_t size)
{
    object_t *ptr = _new_object_tr(vm);
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
