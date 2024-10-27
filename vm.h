#pragma once
#include "stack.h"
#include "object.h"

typedef struct {
    bool debug_mode;
    size_t total_allocations;
    size_t total_frees;
    void **tracked_pointers;  // Array of pointers we've freed
    size_t tracked_count;
    size_t tracked_capacity;
} vm_debug_t;

typedef struct VirtualMachine
{
    stack_t *frames;
    stack_t *objects;
    vm_debug_t *debug;
} vm_t;

typedef struct StackFrame
{
    stack_t *reference;
} frame_t;



vm_t *vm_new(bool debug);
void vm_free(vm_t *vm);
void vm_track_object(vm_t *vm, object_t *obj);

frame_t *vm_new_frame(vm_t *vm);
void vm_frame_push(vm_t *vm, frame_t *frame);
frame_t * vm_frame_pop(vm_t *vm);
void frame_free(frame_t *frame);
void frame_reference_object(frame_t *frame, object_t *obj);

void object_free_tr(vm_t *vm, object_t *obj);

void mark(vm_t *vm);
void trace(vm_t *vm);
void sweep(vm_t *vm);

void vm_collect_garbage(vm_t *vm);

bool vm_debug_was_freed(vm_t *vm, void *ptr);
