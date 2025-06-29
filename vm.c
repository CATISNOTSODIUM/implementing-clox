#include "common.h"
#include "vm.h"
#include "debug.h"

VM vm; // static global VM object

static void resetStack() {
    vm.stack_top = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {
    
}


/**
    run() handles all logics of VM  
*/
static InterpretResult run() {
    if (vm.ip == NULL) {
        return INTERPRET_RUNTIME_ERROR;
    }
    // define some helper functions
    // READ_BYTE reads the current byte code and increment pointer
    #define READ_BYTE() (*vm.ip++) 
    // READ_CONSTANT() returns the value of the constant and increment pointer
    #define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    // BINARY_OP(op) calculates basic arithmetics
    #define BINARY_OP(op) \
    do  {                 \
        double b = pop(); \
        double a = pop(); \
        push(a op b);     \
    } while (false)

    while (true) {
        #ifdef DEBUG_TRACE_EXECUTION
            int offset = vm.ip - vm.chunk->code;
            #ifdef DEBUG_TRACE_STACK_EXECUTION
                printf("[STACK] ");
                for (Value *slot = vm.stack; slot < vm.stack_top; slot++) {
                    printValue(*slot);
                    printf(" ");
                }
                printf("\t");
            #endif
            debugInstruction(vm.chunk, offset);
        #endif

        uint8_t ins;
        switch (ins = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            // Arithmetic expression
            case OP_NEGATE: {
                push(-pop());
                break;
            }
            case OP_ADD:
                BINARY_OP(+);
                break;
            case OP_SUBTRACT:
                BINARY_OP(-);
                break;
            case OP_MULTIPLY:
                BINARY_OP(*);
                break;
            case OP_DIVIDE:
                BINARY_OP(/);
                break;
            case OP_RETURN: {
                return INTERPRET_OK;
            }
        }
    }
    return INTERPRET_OK;

    #undef BINARY_OP
    #undef READ_CONSTANT
    #undef READ_BYTE
}

InterpretResult interpret(const char *source) {
    // Lexing and parsing
    Chunk chunk;
    initChunk(&chunk);
    bool compile_status = compile(source, &chunk); 
    if (!compile_status) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }
    printf("FINISH COMPILING\n");
    debugChunk(&chunk, "Debug after parsing");
    // Load chunk
    vm.chunk = &chunk;
    vm.ip = chunk.code;
    InterpretResult res = run();
    freeChunk(&chunk);
    return res;
}

void push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value pop() {
    if (vm.stack_top != vm.stack) {
        vm.stack_top--;
        return *vm.stack_top;
    } else {
        perror("popping an empty stack is not allowed");
        return 0;
    }
}