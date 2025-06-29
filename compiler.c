#include "compiler.h"
#include "common.h"
#include "lexer.h"
#include "stdio.h"



void eagerLex(const char * source) {
    int line = -1;
    for (;;){
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        }
        else {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR)
            break;
    }
}

void compile(const char *source) {
    initLexer(source);
    eagerLex(source);
}