#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#define STACK_MAX 256

#ifdef DEBUG_TRACE_EXECUTION
  #include "debug.h"
#endif

/**
  VM - A stack-based virtual machine
  @chunk: chunk loaded
  @ip: instruction pointer 
  @stack: a stack array
  @stack_top: a pointer to the top of the stack (unused position)
*/
typedef struct {
  Chunk* chunk;
  uint8_t * ip;
  Value stack[STACK_MAX];
  Value * stack_top;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk * chunk); 

// Stack operations
void push(Value value);
Value pop();

#endif