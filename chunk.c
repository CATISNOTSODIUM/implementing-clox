#include "chunk.h"
#include "memory.h"
#include "common.h"
#include <stdio.h>

void initChunk(Chunk * chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk * chunk, uint8_t byte, int line) {
    // expand its capacity if full
    if (chunk->count + 1 > chunk->capacity) {
        size_t new_size = grow_capacity(chunk->capacity);
        chunk->capacity = new_size;
        chunk->code = (uint8_t *) reallocate(chunk->code, new_size * sizeof(uint8_t));
        chunk->lines = (int *) reallocate(chunk->lines, new_size * sizeof(int));
    }
    
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void freeChunk(Chunk * chunk) {
    freeValueArray(&chunk->constants);
    reallocate(chunk->lines, 0); 
    reallocate(chunk->code, 0);       
}

int addConstant(Chunk * chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1; 
}

void writeConstant(Chunk * chunk, Value value, int line) {
    int loc = addConstant(chunk, value);
    if (loc > UINT8_MAX) {
        fprintf(stderr, "Too many constants in one chunk.");
        return;
    }
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, loc, line);
}