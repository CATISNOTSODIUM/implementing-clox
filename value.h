#ifndef clox_value_h
#define clox_value_h

#include "common.h"


typedef enum {
    VAL_NUMBER,
    VAL_BOOL,
    VAL_NIL,
} ValueType;

/**
 * Value: a value representation with tagged union (roughly takes 16 bytes)
 */
typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as; // A special data type storing boolean and number together
} Value; 

// Macros for value convertor
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
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