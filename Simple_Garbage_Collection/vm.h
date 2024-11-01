#pragma once
#include "stack.h"
#include "object.h"

/**
 * @struct vm_debug_t
 * @brief Structure to store debug information for the virtual machine.
 * 
 * This structure contains debugging flags, counters, and a list of freed pointers
 * for tracking memory management in debug mode.
 */
typedef struct {
    bool debug_mode;           /**< Flag indicating if debug mode is enabled */
    size_t total_allocations;  /**< Total number of allocations */
    size_t total_frees;        /**< Total number of frees */
    void **tracked_pointers;   /**< Array of pointers that have been freed */
    size_t tracked_count;      /**< Number of tracked pointers */
    size_t tracked_capacity;   /**< Capacity of the tracked pointers array */
} vm_debug_t;

/**
 * @struct VirtualMachine
 * @brief The main virtual machine structure.
 * 
 * Holds the VM's stack frames, objects, and debug information.
 */
typedef struct VirtualMachine {
    stack_t *frames;       /**< Stack of frames in the virtual machine */
    stack_t *objects;      /**< Stack of objects managed by the virtual machine */
    vm_debug_t *debug;     /**< Debug information, if debug mode is enabled */
} vm_t;

/**
 * @struct StackFrame
 * @brief Represents a frame in the virtual machine's stack.
 */
typedef struct StackFrame {
    stack_t *reference;    /**< Reference to the stack */
} frame_t;

/**
 * @brief Initialize a new virtual machine.
 * 
 * @param debug Boolean flag to enable or disable debug mode.
 * @return Pointer to the newly created virtual machine instance.
 */
vm_t *vm_new(bool debug);

/**
 * @brief Free the virtual machine and its resources.
 * 
 * @param vm Pointer to the virtual machine to free.
 */
void vm_free(vm_t *vm);

/**
 * @brief Track an object within the virtual machine.
 * 
 * @param vm Pointer to the virtual machine.
 * @param obj Object to be tracked by the VM.
 */
void vm_track_object(vm_t *vm, object_t *obj);

/**
 * @brief Create a new stack frame in the virtual machine.
 * 
 * @param vm Pointer to the virtual machine.
 * @return Pointer to the newly created frame.
 */
frame_t *vm_new_frame(vm_t *vm);

/**
 * @brief Pop a frame from the virtual machine's frame stack.
 * 
 * @param vm Pointer to the virtual machine.
 * @return Pointer to the popped frame.
 */
frame_t *vm_frame_pop(vm_t *vm);

/**
 * @brief Reference an object in a frame.
 * 
 * @param frame Pointer to the frame.
 * @param obj Object to be referenced within the frame.
 */
void frame_reference_object(frame_t *frame, object_t *obj);

/**
 * @brief Free a frame and its associated resources.
 * 
 * @param frame Pointer to the frame to free.
 */
void frame_free(frame_t *frame);

/**
 * @brief Run garbage collection on the virtual machine.
 * 
 * @param vm Pointer to the virtual machine.
 */
void vm_collect_garbage(vm_t *vm);

/**
 * @brief Check if a pointer has been freed in debug mode.
 * 
 * @param vm Pointer to the virtual machine.
 * @param ptr Pointer to check if it was previously freed.
 * @return True if the pointer was freed, false otherwise.
 */
bool vm_debug_was_freed(vm_t *vm, void *ptr);
