# Notes I've taken during reading the [book](https://craftinginterpreters.com)
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

## Virtual Machine (VM)
Given an array of bytecodes, a mixture between operation code and location of values from constant pool, we use VM to traverse along this array and evaluate the program.

We can achieve this by using a stack-based approach. This is quite similar to what we do with recursion-based AST evaluation, but we use stack to minimize traversal within a heap.

Here is how our VM evaluate simple arithmetics. Given that your byte code is `[1, 2, +]`, the first two elements of the array (operands) are loaded into the stack before being evaluated by the operator `+`.
The content of the stack can be visualised as follows.

``` 
[1]         1 is loaded.
[1, 2]      2 is loaded.
[3]         Encounter +, pop the last two elements and replace with the result.
```
You can enable `DEBUG_TRACE_STACK_EXECUTION` flag (in Makefile) in order to track the content inside the stack.  

## Lexing
Lexing is tedious. Nothing interesting here. 
```c
typedef struct {
    const char * start;
    const char * current;
    uint line;
} Lexer;

// global variables
Lexer lexer; 
Trie identifierTrie;
```
The only thing that is different from the book is that I decided to use `Trie` to store all keywords and their corresponding tokens instead of hardcoded them.
```c
struct Trie {
    bool is_key;
    void * value;
    Trie * children[TRIE_NUM_KEY];
};
```

## Compiling
At this stage, we have an array of tokens. We want to convert them to an array of bytecodes, such that these values can be processed by our VM.