#include "common.h"
#include "chunk.h"
#include "debug.h"

int main() {
    Chunk chunk;
    initChunk(&chunk);

    writeConstant(&chunk, 5, 1);

    debugChunk(&chunk, "first chunk");
    freeChunk(&chunk);
    return 0;
}