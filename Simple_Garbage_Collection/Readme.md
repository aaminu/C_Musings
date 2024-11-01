# Simple Garbage Collection (GC) System written in C

This repository implements a sample garbage collection system in C, similar to those used in languages with built-in GC, utilizing two primary memory management techniques.: **Reference Counting** and **Mark-and-Sweep Garbage Collection**. This setup is ideal for managing dynamically allocated objects. 

## Features

- **Reference Counting (RC)**: Keeps track of the number of references to each object and automatically deallocates objects when they are no longer in use i.e when the reference gets to zero.
- **Mark-and-Sweep (M&S)**: Traverses all reachable objects from a set of root objects, marking them as in-use. Unreachable objects are then swept (deallocated).

## Files Overview

### Core Components

- **`main.c`**: Demonstrates how to each of the implementation would be used in a behind the scene fashion.
- **`vm.h` and `vm.c`**: Implements a simple VM structure and garbage collection system. It includes:
  - Initialization and cleanup routines.
  - Object tracking and management.
  - Garbage collection functions using mark-and-sweep.
  - Debug features for tracking memory frees in debug mode.
- **`object_ms.h` and `object_ms.c`**: Handles the object creation that use mark and sweep mechanism.
- **`object_rc.h` and `object_rc.c`**: Manages objects using reference counting, incrementing and decrementing reference counts as objects are created and destroyed.

- **`object.h`**: Defines the `object_t` structure, the basis of objects managed by the VM.
- **`stack.h` and `stack.c`**: Provides a simple stack data structure to support frame and object management in the VM.
- **`munit.h` and `munit.c`**: Unit testing framework

### Memory Management

This system utilizes two garbage collection strategies:
- **Reference Counting**: Each object keeps a `refcount` that increments when a new reference to the object is created and decrements when a reference is removed. When `refcount` reaches zero, the object is freed immediately.
- **Mark-and-Sweep**: To handle cyclic dependencies, the VM periodically executes a mark-and-sweep cycle, marking all reachable objects and deallocating those that are unreachable. This is essential for cleaning up objects that cannot be freed by reference counting alone.

### Usage

1. **Compile**: Compile the files manually with `gcc`:
   ```bash
   gcc -o vm main.c vm.c stack.c object_rc.c object_ms.c
   ```

### Credit
- [Boot.Dev](https://www.boot.dev/)
