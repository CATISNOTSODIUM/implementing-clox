#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"
size_t grow_capacity(size_t old_size);

// void * is a generic pointer
void * reallocate(void * ptr, size_t new_size);

#endif