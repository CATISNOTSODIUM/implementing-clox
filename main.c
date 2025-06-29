#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include "trie.h"
static void repl(){
    char buf[1024];
    while (true) {
        printf("$ ");
        if (!fgets(buf, sizeof(buf), stdin)){
            printf("\n");
            break;
        }
        interpret(buf);
    }
}

int main(int argc, const char *argv[]) {
    // Initialize
    initVM();
    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        fprintf(stderr, "Not implemented yet\n");
        // runFile(argv[1]);
    } else {
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }
    // Free
    freeVM();
    return 0;
}