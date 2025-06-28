# Notes I've taken during reading the book
## The inspiration behinds Bytecode representation
AST based interpreter is slow because it is not memory-efficient. Each piece of syntax becomes an AST node stored in a heap with lots of pointers between them. All of these problems are not cache-friendly.

Our goal is to store the instruction in a dense-contiguous manner, in such way that when the CPU reads a bit of data from RAM, it also pulls in its adjacent instructions into cache. 

## Representing instruction
To represent operation code, we use enum `OpCode` to handle the operation code abstration. Note that we don't exactly know the size of the operation code until we know exactly how many instructions we are included.

To store a series of instructions, we use **dynamic array**. Since we are in C, we have to build it by ourselves.  

## Constant pool
We (again) use dynamic array to store values used in the program. This is called a constant pool.

Adding constant
```C
// BYTE: [const] [loc_constant_pool]
writeChunk(&chunk, OP_CONSTANT);
writeChunk(&chunk, addConstant(&chunk, 1.2));
```