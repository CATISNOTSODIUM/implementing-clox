#ifndef clox_value_h
#define clox_value_h

#include "common.h"
typedef double Value;

// Constant pool
typedef struct {
    size_t count;
    size_t capacity;
    Value * values;
} ValueArray;

void initValueArray(ValueArray * arr);
void writeValueArray(ValueArray * arr, Value value);
void freeValueArray(ValueArray * arr);
#endif