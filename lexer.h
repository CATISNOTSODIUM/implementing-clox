#ifndef clox_lexer_h
#define clox_lexer_h
#include "common.h"

typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

    TOKEN_ERROR, TOKEN_EOF
} TokenType;

static TokenType keyword_tokens[] = {
    TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE};

static const char *keyword_strings[] = {
    "and", "class", "else", "false",
    "for", "fun", "if", "nil", "or",
    "print", "return", "super", "this",
    "true", "var", "while"};

/**
 * Token
 * @start: pointer points to the start of the slice 
 * @length: length of the slice
 * @line: line number
 */
typedef struct {
    TokenType type;
    const char * start;
    int length;
    int line;
} Token;

/**
 * Lexer - a (lazy) scanner
 * @start: pointer points to the start of the current lexeme
 * @current: pointer points to the current character
 * @line: line number
 */
typedef struct {
    const char * start;
    const char * current;
    uint line;
} Lexer;

void initLexer(const char *source);
Token scanToken();
void freeLexer();

#endif