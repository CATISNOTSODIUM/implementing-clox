#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
#include "common.h"
#include "lexer.h"
#include "chunk.h"
#include "debug.h"

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

/**
 * Pratt Parser
 * @current: the current token (next token to be consumed)
 * @previous: the token the parser has just consumed
 * @had_error: flag indicating whether the parser has error or not
 * @panic_mode: a flag indicating that the parser already has an error
 * and suppress the remaining warnings until the end of the statement
*/
typedef struct{
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
} Parser;

typedef enum {
    PREC_NONE,       // Lowest
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY     // Highest
} Precedence;

typedef void (*ParseFn)();
/**
 * Pratt parse rule
 * ParseFn is a function ptr type () => {} 
 */
typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

Parser parser;
Chunk * compilingChunk;

static Chunk * currentChunk() {
    return compilingChunk;
}

static void errorAt(Token *token, const char *message) {
    if (parser.panic_mode) return;
    parser.panic_mode = true;
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR) {
        // Nothing.
    }
    else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.had_error = true;
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

static void error(const char *message) {
    errorAt(&parser.previous, message);
}


static void advance() {
    parser.previous = parser.current;
    // Keep reading until hitting non-error token
    while (true) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TokenType type, const char * msg) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    errorAtCurrent(msg);
}

/**
 * emitByte - add byte to chunk
 */
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitReturn() {
    emitByte(OP_RETURN);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitConstant(Value value) {
    writeConstant(currentChunk(), value, parser.previous.line);
}

static void endCompiler() {
    emitReturn();
}

static void expression();
static ParseRule *getRule(TokenType type);
static void parsePrecedence(Precedence precedence);


static void parsePrecedence(Precedence precedence) {
    advance(); 
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression");
        return;
    }
    prefixRule();
    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

static void binary() {
    // Up to this point, the left operand has already been parsed
    TokenType operatorType = parser.previous.type;
    ParseRule *rule = getRule(operatorType);
    // Parse right operand
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
    case TOKEN_PLUS:
        emitByte(OP_ADD);
        break;
    case TOKEN_MINUS:
        emitByte(OP_SUBTRACT);
        break;
    case TOKEN_STAR:
        emitByte(OP_MULTIPLY);
        break;
    case TOKEN_SLASH:
        emitByte(OP_DIVIDE);
        break;
    default:
        return; // Unreachable.
    }
}



/**
 * expression() - parsing expression
 */
static void expression() {
    // entry point: PREC_ASSIGNMENT < PREC_OR (lowest precedence among expression)
    parsePrecedence(PREC_ASSIGNMENT);
}

static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

// grouping assumes that the TOKEN_LEFT_PAREN has already been consumed
static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void unary() {
    // Upon calling this function, operator has already been parsed
    TokenType type = parser.previous.type;
    // Parse the operand (only with precendence higher than unary operator)
    parsePrecedence(PREC_UNARY);
    // After calling this function, the operand has already been added 
    // into a chunk before the operator has been loaded. (postfix)
    switch (type) {
        case TOKEN_MINUS:
            emitByte(OP_NEGATE);
            break;
        case TOKEN_BANG:
            emitByte(OP_NOT);
            break;
        default:
            return;
    }
}

static void literal() {
    switch (parser.previous.type) {
        case TOKEN_FALSE:
            emitByte(OP_FALSE);
            break;
        case TOKEN_NIL:
            emitByte(OP_NIL);
            break;
        case TOKEN_TRUE:
            emitByte(OP_TRUE);
            break;
        default:
            return; // Unreachable.
    }
}

ParseRule rules[] = {
    // TokenType = {prefix, infix, precedence level}
    [TOKEN_LEFT_PAREN] = {grouping, NULL, PREC_NONE},
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
    [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
    [TOKEN_MINUS] = {unary, binary, PREC_TERM},
    [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
    [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
    [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
    [TOKEN_BANG] = {unary, NULL, PREC_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
    [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
    [TOKEN_AND] = {NULL, NULL, PREC_NONE},
    [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FALSE] = {literal, NULL, PREC_NONE},
    [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
    [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
    [TOKEN_IF] = {NULL, NULL, PREC_NONE},
    [TOKEN_NIL] = {literal, NULL, PREC_NONE},
    [TOKEN_OR] = {NULL, NULL, PREC_NONE},
    [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
    [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
    [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
    [TOKEN_TRUE] = {literal, NULL, PREC_NONE},
    [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
    [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};

static ParseRule *getRule(TokenType type) {
    return &rules[type];
}

bool compile(const char *source, Chunk * chunk) {
    initLexer(source);
    compilingChunk = chunk;
    parser.had_error = false;
    parser.panic_mode = false;
    advance(); // get token type of the first token (to initiate consume())
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
    freeLexer(); // free lexer (including trie) after finish compiling
    return !parser.had_error;
}