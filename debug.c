#include "debug.h"

void debugChunk(Chunk * chunk, const char * name) {
    printf("[DEBUG] %s\n", name);
    for (int i = 0; i < (int) chunk->count; ) {
        // read the instruction and offset the counter
        i = debugInstruction(chunk, i);
    }
}

/**
    simpleInstruction (static) prints out instruction 
*/

void printValue(Value value) {
    printf("%g", value);
}

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk * chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%s: ", name);
    printValue(chunk->constants.values[constant]);
    printf("\n");
    return offset + 2;
}

int debugInstruction(Chunk * chunk, int offset) {
    uint8_t instruction = chunk->code[offset];
    printf("%04d ", offset);
    if (offset > 0 &&
        chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->lines[offset]);
    }
    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown opcode %d\n", instruction);
        // proceed to next instruction
        return offset + 1;
    }
}