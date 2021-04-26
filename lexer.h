#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>
#include <vector>

#include "inputBuffer.h"

typedef enum
{
    END_OF_FILE = 0,
    ABSTRACT,
    ARRAY,
    AS,
    BREAK,
    CASE,
    CATCH,
    CLASS,
    CLONE,
    CONST,
    CONTINUE,
    DECLARE,
    DEFAULT,
    DO,
    ELSE,
    ELSE_IF,
    END_DECLARE,
    END_FOR,
    END_FOREACH,
    END_IF,
    END_SWITCH,
    END_WHILE,
    EXTENDS,
    FINAL,
    FOR,
    FOR_EACH,
    FUNCTION,
    GLOBAL,
    GO_TO,
    IF,
    IMPLEMENTS,
    INTERFACE,
    INSTANCE_OF,
    NAMESPACE,
    NEW,
    PRIVATE,
    PROTECTED,
    PUBLIC,
    STATIC,
    SWITCH,
    THROW,
    TRY,
    USE,
    VAR,
    WHILE,
    YIELD,
    FROM,
    RETURN,
    ADDITION,
    SUBSTRACTION,
    MULTIPLICATION,
    DIVISION,
    MODULUS,
    EXPONENTIATION,
    EQUAL,
    IDENTICAL,
    NOT_EQUAL,
    NOT_IDENTICAL,
    GREATER_THAN,
    LESS_THAN,
    GREATER_EQUAL,
    LESS_EQUAL,
    SPACESHIP,
    INCREMENT,
    DECREMENT,
    AND,
    OR,
    XOR,
    NOT,
    COMMENT,
    DOLLAR,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    LEFT_SQUARE,
    RIGHT_SQUARE,
    LEFT_CURLY,
    RIGHT_CURLY,
    LEFT_PARENTHESE,
    RIGHT_PARENTHESE,
    DOT,
    COMMA,
    COLON,
    SEMICOLON,
    IDENTIFIER,
    QUESTION_MARK,
    NUMBER,
    REALNUM,
    ERROR
} TokenType;

class Token
{
public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer
{
public:
    Token getToken();
    TokenType ungetToken(Token);
    LexicalAnalyzer();

private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool skipSpace();
    bool isIdChar(char);
    bool isKeyword(std::string);
    TokenType getKeywordIndex(std::string);
    Token scanIdOrKeyword();
    Token scanNumber();
};

#endif //__LEXER_H__