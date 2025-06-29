#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main() {
    // Initialize
    initVM();
    Chunk chunk;
    initChunk(&chunk);
  
    writeConstant(&chunk, 5, 1);
    writeConstant(&chunk, 3, 1);
    writeChunk(&chunk, OP_ADD, 1);
    writeChunk(&chunk, OP_RETURN, 2);

    // debugChunk(&chunk, "first chunk");
    interpret(&chunk);
    // Free
    freeVM();
    freeChunk(&chunk);
    return 0;
}