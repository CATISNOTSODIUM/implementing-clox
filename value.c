#include "value.h"
#include "memory.h"

void initValueArray(ValueArray * arr) {
    arr->count = 0;
    arr->capacity = 0;
    arr->values = NULL;
}

void writeValueArray(ValueArray * arr, Value value) {
    // expand its capacity if full
    if (arr->count + 1 > arr->capacity) {
        size_t new_capacity = grow_capacity(arr->capacity); 
        arr->values = (Value *) reallocate(arr->values, new_capacity);       
    }
    arr->values[arr->count] = value;
    arr->count++;
}

void freeValueArray(ValueArray * arr) {
    reallocate(arr->values, 0);       
}