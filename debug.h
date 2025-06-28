#ifndef clox_debug_h
#define clox_debug_h

#include "common.h"
#include "chunk.h"
#include "stdio.h" // for debugging

/**
    debugChunk - print out the current instructions in the dynamic array
    @chunk: chunk pointer
    @name: name of the chunk (forsake of debugging)
*/
void debugChunk(Chunk * chunk, const char * name);

/**
    debugInstruction - scan the next instruction and adjust the offset
    @chunk: chunk pointer
    @offset: previous offset
*/
int debugInstruction(Chunk * chunk, int offset);

#endif