#include "memory.h"

#include "stdio.h"

size_t grow_capacity(size_t old_size) {
    return old_size < 8
        ? 8
        : 2 * old_size;
}

void * reallocate(void * ptr, size_t new_size) {
    
    if (new_size == 0) {
        free(ptr);
        return NULL; 
    }
    void * newptr = realloc(ptr, new_size);
    return newptr;
}