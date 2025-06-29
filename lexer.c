#include "lexer.h"
#include "string.h"
#include "trie.h"

Lexer lexer; 
Trie identifierTrie;

bool isEOF() {
    return *lexer.current == '\0';
}

void initLexer(const char *source) {
    lexer.start = source;
    lexer.current = source;
    lexer.line = 1;
    // Add relevant identifiers
    initTrie(&identifierTrie);
    for (int i = 0; i < sizeof(keyword_tokens)/sizeof(TokenType); i++) {
        insertTrie(&identifierTrie, keyword_strings[i], &keyword_tokens[i]);
    }
}

void freeLexer() {
    freeTrie(&identifierTrie);
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer.start;
    token.length = (int)(lexer.current - lexer.start);
    token.line = lexer.line;
    return token;
}

static Token errorToken(const char * msg) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = msg;
    token.length = (int) strlen(msg);
    token.line = lexer.line;
    return token;
}

static char advance() {
    char read = lexer.current[0];
    lexer.current++;
    return read;
}

static char peek() {
    char read = lexer.current[0];
    return read;
}

static char peekNext() {
    return isEOF() ? '\0' : lexer.current[1];
}

static void skipWhitespace() {
    while (true) {
        switch (peek()) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                lexer.line++;
                advance();
                break;
            case '#':
                while (peek() != '\n' && !isEOF()) advance();
            default:
                return;
        }
    }
}

static Token string() {
    while (peek() != '"' && !isEOF()) {
        if (peek() == '\n') lexer.line++;
        advance();
    }

    if (isEOF()) return errorToken("Unterminated string.");

    advance();
    return makeToken(TOKEN_STRING);
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

static Token number() {
    while (isDigit(peek()))
        advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the ".".
        advance();
        while (isDigit(peek()))
            advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static TokenType identifierType() {
    uint length = lexer.current - lexer.start;
    char * keyword = malloc(length + 1);
    strncpy(keyword, lexer.start, length);
    TokenType *token = (TokenType *) searchTrie(&identifierTrie, keyword);
    return token == NULL ? TOKEN_IDENTIFIER : *token; 
}

static Token identifier() {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

Token scanToken() {
    skipWhitespace();
    lexer.start = lexer.current;
    if (isEOF()) return makeToken(TOKEN_EOF);

    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();
    switch (c) {
        case '(':
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            return makeToken(TOKEN_RIGHT_PAREN);
        case '{':
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            return makeToken(TOKEN_RIGHT_BRACE);
        case ';':
            return makeToken(TOKEN_SEMICOLON);
        case ',':
            return makeToken(TOKEN_COMMA);
        case '.':
            return makeToken(TOKEN_DOT);
        case '-':
            return makeToken(TOKEN_MINUS);
        case '+':
            return makeToken(TOKEN_PLUS);
        case '/':
            return makeToken(TOKEN_SLASH);
        case '*':
            return makeToken(TOKEN_STAR);
        // string
        case '"':
            return string();
    }
    return errorToken("Unexpected character");
}