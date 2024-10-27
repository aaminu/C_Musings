#include "vm.h"

void vm_debug_init(vm_t *vm)
{
    vm->debug = malloc(sizeof(vm_debug_t));
    if (vm->debug)
    {
        vm->debug->debug_mode = true;
        vm->debug->total_allocations = 0;
        vm->debug->total_frees = 0;
        vm->debug->tracked_capacity = 64; // Initial capacity
        vm->debug->tracked_count = 0;
        vm->debug->tracked_pointers = calloc(vm->debug->tracked_capacity, sizeof(void *));
    }
}

vm_t *vm_new(bool debug)
{
    vm_t *vm = malloc(sizeof(vm_t));
    if (vm == NULL)
        return NULL;

    vm->frames = stack_new(8);
    if (vm->frames == NULL)
    {
        free(vm);
        return NULL;
    }
    vm->objects = stack_new(8);
    if (vm->objects == NULL)
    {
        stack_free(vm->frames);
        free(vm);
        return NULL;
    }

    if (debug)
        vm_debug_init(vm);

    return vm;
}

void vm_debug_track_free(vm_t *vm, void *ptr)
{
    if (vm->debug == NULL || !vm->debug->debug_mode)
        return;

    vm->debug->total_frees++;

    // Track the freed pointer
    if (vm->debug->tracked_count >= vm->debug->tracked_capacity)
    {
        size_t new_capacity = vm->debug->tracked_capacity * 2;
        void **new_array = realloc(vm->debug->tracked_pointers,
                                   sizeof(void *) * new_capacity);
        if (!new_array)
            return;

        vm->debug->tracked_pointers = new_array;
        vm->debug->tracked_capacity = new_capacity;
    }

    vm->debug->tracked_pointers[vm->debug->tracked_count++] = ptr;
}

bool vm_debug_was_freed(vm_t *vm, void *ptr)
{
    if (vm->debug == NULL || !vm->debug->debug_mode)
        return false;

    for (size_t i = 0; i < vm->debug->tracked_count; i++)
    {
        if (vm->debug->tracked_pointers[i] == ptr)
        {
            return true;
        }
    }
    return false;
}

void vm_debug_cleanup(vm_t *vm)
{
    if (vm->debug)
    {
        free(vm->debug->tracked_pointers);
        free(vm->debug);
    }
}

void vm_free(vm_t *vm)
{
    vm_debug_cleanup(vm);

    for (int i = 0; i < vm->frames->count; i++)
    {
        frame_free(vm->frames->data[i]);
    }
    stack_free(vm->frames);

    for (int i = 0; i < vm->objects->count; i++)
    {
        object_free_tr(vm, vm->objects->data[i]);
    }
    stack_free(vm->objects);
    free(vm);
}

void vm_track_object(vm_t *vm, object_t *obj)
{
    if (vm == NULL || obj == NULL)
    {
        return;
    }
    stack_push(vm->objects, obj);
}

void vm_frame_push(vm_t *vm, frame_t *frame)
{
    if (frame == NULL || vm == NULL)
    {
        return;
    }

    stack_push(vm->frames, frame);
}

frame_t *vm_frame_pop(vm_t *vm)
{
    if (vm == NULL)
    {
        return NULL;
    }

    return stack_pop(vm->frames);
}

frame_t *vm_new_frame(vm_t *vm)
{
    frame_t *frame = malloc(sizeof(frame_t));
    if (frame == NULL)
    {
        return NULL;
    }

    frame->reference = stack_new(8);
    if (frame->reference == NULL)
    {
        free(frame);
        return NULL;
    }

    vm_frame_push(vm, frame);
    return frame;
}

void frame_free(frame_t *frame)
{
    stack_free(frame->reference);
    free(frame);
}

void object_free_tr(vm_t *vm, object_t *obj)
{
    if (obj == NULL)
        return;

    switch (obj->kind)
    {
    case INTEGER:
    case FLOAT:
        break;

    case STRING:
        // Free the dynamically allocated string,
        free(obj->data.v_string);
        break;

    case VECTOR3:
        break;

    case ARRAY:
        if (obj->data.v_array.elements != NULL)
        {
            free(obj->data.v_array.elements);
        }
        break;

    default:
        break;
    }

    vm_debug_track_free(vm, obj);
    free(obj);
}

void frame_reference_object(frame_t *frame, object_t *obj)
{
    if (frame == NULL || obj == NULL)
    {
        return;
    }

    stack_push(frame->reference, obj);
}

void mark(vm_t *vm)
{
    for (size_t i = 0; i < vm->frames->count; i++)
    {
        frame_t *frame = vm->frames->data[i];
        for (size_t j = 0; j < frame->reference->count; j++)
        {
            object_t *obj = frame->reference->data[j];
            obj->is_marked = true;
        }
    }
}

void trace_mark_object(stack_t *gray_objects, object_t *obj)
{
    if (obj == NULL || obj->is_marked)
        return;

    obj->is_marked = true;
    stack_push(gray_objects, obj);
}

void trace_blacken_object(stack_t *gray_objects, object_t *obj)
{
    if (obj == NULL)
        return;

    switch (obj->kind)
    {
    case INTEGER:
    case FLOAT:
    case STRING:
        /* code */
        break;

    case VECTOR3:
        trace_mark_object(gray_objects, obj->data.v_vector3.x);
        trace_mark_object(gray_objects, obj->data.v_vector3.y);
        trace_mark_object(gray_objects, obj->data.v_vector3.z);
        break;

    case ARRAY:
        for (size_t i = 0; i < obj->data.v_array.size; i++)
        {
            trace_mark_object(gray_objects, obj->data.v_array.elements[i]);
        }
        break;

    default:
        break;
    }
}

void trace(vm_t *vm)
{
    stack_t *gray_objects = stack_new(8);
    if (gray_objects == NULL)
        return;

    for (int i = 0; i < vm->objects->count; i++)
    {
        object_t *obj = vm->objects->data[i];
        if (obj == NULL)
            continue;
        if (obj->is_marked)
        {
            stack_push(gray_objects, obj);
        }
    }
    while (gray_objects->count > 0)
    {
        trace_blacken_object(gray_objects, stack_pop(gray_objects));
    }

    stack_free(gray_objects);
}

void sweep(vm_t *vm)
{
    for (int i = 0; i < vm->objects->count; i++)
    {
        object_t *obj = vm->objects->data[i];
        if (obj == NULL)
            continue;
        if (obj->is_marked)
        {
            obj->is_marked = false;
        }
        else
        {
            object_free_tr(vm, obj);
            vm->objects->data[i] = NULL;
        }
    }

    // TODO: remove Nulls
}

void vm_collect_garbage(vm_t *vm)
{
    mark(vm);
    trace(vm);
    sweep(vm);
}
