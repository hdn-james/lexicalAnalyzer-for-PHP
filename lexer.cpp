#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputBuffer.h"

using namespace std;

#define KEYWORDS_COUNT 47
string keyword[] = {
    "abstract", "array()", "as", "break", "case", "catch", "class", "clone",
    "const", "continue", "declare", "default", "do", "else", "elseif", "enddeclare", "endfor", "endforeach",
    "endif", "endswitch", "endwhile", "extends", "final", "for", "foreach", "function", "global", "goto",
    "if", "implements", "interface", "instanceof", "namespace", "new", "private", "protected", "public",
    "static", "switch", "throw", "try", "use", "var", "while", "yield", "from", "return"};

string reserved[] = {
    "END_OF_FILE", "ABSTRACT", "ARRAY", "AS", "BREAK", "CASE", "CATCH", "CLASS", "CLONE",
    "CONST", "CONTINUE", "DECLARE", "DEFAULT", "DO", "ELSE", "ELSE_IF", "END_DECLARE", "END_FOR", "END_FOREACH",
    "END_IF", "END_SWITCH", "END_WHILE", "EXTENDS", "FINAL", "FOR", "FOR_EACH", "FUNCTION", "GLOBAL", "GO_TO",
    "IF", "IMPLEMENTS", "INTERFACE", "INSTANCE_OF", "NAMESPACE", "NEW", "PRIVATE", "PROTECTED", "PUBLIC",
    "STATIC", "SWITCH", "THROW", "TRY", "USE", "VAR", "WHILE", "YIELD", "FROM",
    "RETURN", "ADDITION", "SUBSTRACTION", "MULTIPLICATION", "DIVISION", "MODULUS", "EXPONENTIATION",
    "EQUAL", "IDENTICAL", "NOT_EQUAL", "NOT_IDENTICAL", "GREATER_THAN", "LESS_THAN", "GREATER_EQUAL",
    "LESS_EQUAL", "SPACESHIP", "INCREMENT", "DECREMENT", "AND", "OR", "XOR", "NOT",
    "COMMENT", "DOLLAR", "SINGLE_QUOTE", "DOUBLE_QUOTE", "LEFT_SQUARE", "RIGHT_SQUARE",
    "LEFT_CURLY", "RIGHT_CURLY", "LEFT_PARENTHESE", "RIGHT_PARENTHESE",
    "DOT", "COMMA", "COLON", "SEMICOLON", "IDENTIFIER",
    "QUESTION_MARK", "NUMBER", "REALNUM", "ERROR"};

void Token::Print()
{
    cout
        << "{" << this->lexeme << " , "
        << reserved[(int)this->token_type] << " , "
        << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::skipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c))
    {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput())
        input.UngetChar(c);
    return space_encountered;
}

bool LexicalAnalyzer::isIdChar(char c)
{
    return (isalnum(c) || c == '_' || c == '(' || c == ')' || (c >= 127 && c <= 255));
}

bool LexicalAnalyzer::isKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
        if (s == keyword[i])
            return true;
    return false;
}

TokenType LexicalAnalyzer::getKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++)
        if (s == keyword[i])
            return (TokenType)(i + 1);
    return ERROR;
}

Token LexicalAnalyzer::scanIdOrKeyword()
{
    char c;
    input.GetChar(c);
    if (isalpha(c))
    {
        tmp.lexeme = "";
        while (!input.EndOfInput() && (isIdChar(c)))
        {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (isKeyword(tmp.lexeme))
            tmp.token_type = getKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = IDENTIFIER;
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

TokenType LexicalAnalyzer::ungetToken(Token t)
{
    tokens.push_back(t);
    return t.token_type;
}

Token LexicalAnalyzer::getToken()
{
    char c;
    if (!tokens.empty())
    {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    skipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c)
    {
    case '{':
        tmp.token_type = LEFT_CURLY;
        return tmp;
    case '}':
        tmp.token_type = RIGHT_CURLY;
        return tmp;
    case '\'':
        tmp.token_type = SINGLE_QUOTE;
        return tmp;
    case '\"':
        tmp.token_type = DOUBLE_QUOTE;
        return tmp;
    case '$':
        tmp.token_type = DOLLAR;
        return tmp;
    case '|':
        input.GetChar(c);
        if (c == '|')
            tmp.token_type = OR;
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = ERROR;
        }
        return tmp;
    case '!':
        input.GetChar(c);
        if (c == '=')
            tmp.token_type = NOT_EQUAL;
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = NOT;
        }
        return tmp;
    case '&':
        input.GetChar(c);
        if (c == '&')
            tmp.token_type = AND;
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = ERROR;
        }
        return tmp;
    case '?':
        tmp.token_type = QUESTION_MARK;
        return tmp;
    case '.':
        tmp.token_type = DOT;
        return tmp;
    case '+':
        input.GetChar(c);
        if (c == '+')
        {
            tmp.token_type = INCREMENT;
            return tmp;
        }
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = ADDITION;
            return tmp;
        }
    case '-':
        input.GetChar(c);
        if (c == '-')
        {
            tmp.token_type = DECREMENT;
            return tmp;
        }
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = SUBSTRACTION;
            return tmp;
        }
    case '/':
        input.GetChar(c);
        if (c == '/')
        {
            tmp.token_type = COMMENT;
        }
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = DIVISION;
        }
        return tmp;
    case '*':
        input.GetChar(c);
        if (c == '*')
        {
            tmp.token_type = EXPONENTIATION;
            return tmp;
        }
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = MULTIPLICATION;
        }
        return tmp;
    case '=':
        tmp.token_type = EQUAL;
        return tmp;
    case ':':
        tmp.token_type = COLON;
        return tmp;
    case ',':
        tmp.token_type = COMMA;
        return tmp;
    case ';':
        tmp.token_type = SEMICOLON;
        return tmp;
    case '[':
        tmp.token_type = LEFT_SQUARE;
        return tmp;
    case ']':
        tmp.token_type = RIGHT_SQUARE;
        return tmp;
    case '(':
        tmp.token_type = LEFT_PARENTHESE;
        return tmp;
    case ')':
        tmp.token_type = RIGHT_PARENTHESE;
        return tmp;
    case '<':
        input.GetChar(c);
        if (c == '=')
            tmp.token_type = LESS_EQUAL;
        else if (c == '>')
            tmp.token_type = NOT_EQUAL;
        else
        {
            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
            tmp.token_type = LESS_THAN;
        }
        return tmp;
    case '>':
        input.GetChar(c);
        if (c == '=')
            tmp.token_type = GREATER_EQUAL;
        else
        {
            if (!input.EndOfInput())
                input.UngetChar(c);
            tmp.token_type = GREATER_THAN;
        }
        return tmp;
    default:
        if (isdigit(c))
        {
            input.UngetChar(c);
            return scanNumber();
        }
        else if (isalpha(c))
        {
            input.UngetChar(c);
            return scanIdOrKeyword();
        }
        else if (input.EndOfInput())
            tmp.token_type = END_OF_FILE;
        else
            tmp.token_type = ERROR;

        return tmp;
    }
}

Token LexicalAnalyzer::scanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c))
    {
        if (c == '0')
        {
            tmp.lexeme = "0";
        }
        else
        {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c))
            {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        input.GetChar(c);
        // cout << "LOG: " << c << endl;
        if (c == '.')
        {
            do
            {
                tmp.lexeme += c;
                input.GetChar(c);
            } while (!input.EndOfInput() && isdigit(c));

            if (!input.EndOfInput())
            {
                input.UngetChar(c);
            }
            tmp.token_type = REALNUM;
            tmp.line_no = line_no;
        }
        else
        {
            tmp.token_type = NUMBER;
            tmp.line_no = line_no;
        }
        return tmp;
    }
    else
    {
        if (!input.EndOfInput())
        {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.getToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.getToken();
        token.Print();
    }
}