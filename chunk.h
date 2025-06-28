#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_RETURN,
} OpCode;


// Dynamic array
typedef struct {
    size_t count;
    size_t capacity;
    uint8_t * code;
    int * lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk * chunk);
void writeChunk(Chunk * chunk, uint8_t byte, int line);
void freeChunk(Chunk * chunk);

/**
    addConstant: add value to the constant pool
    @chunk: chunk pointer
    @value: value
    @return: location of the constant value in an array
*/
int addConstant(Chunk * chunk, Value value);
void writeConstant(Chunk * chunk, Value value, int line);
#endif