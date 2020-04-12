#ifndef RSLLEX_HPP_
#define RSLLEX_HPP_

#include <string>
#include <vector>

namespace rsl {

enum token_t
{
    TK_LINECOMMENT,
    TK_MULTICOMMENT,
    TK_IDENTIFIER,
    TK_CLASS,
    TK_STRUCT,
    TK_BINOP,
    TK_STATIC,
    TK_THIS,
    TK_COMMA,
    TK_COLON,
    TK_TYPEDEF,
    TK_EOF,
    TK_OPENPAREN,
    TK_CLOSEPAREN,
    TK_OPENBRACKET,
    TK_CLOSEBRACKET,
    TK_SEMICOLON,
    TK_STRINGLITERAL,
    TK_INTLITERAL,
    TK_FLOATLITERAL,
    TK_OPERATOR,
    TK_CONST,
    TK_NEW,
    TK_DELETE,
    TK_SIZEOF,
    TK_TYPEOF,
    TK_AND,
    TK_IS,
    TK_ISNOT,
    TK_NOT,
    TK_FUNCTION,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_SWITCH,
    TK_CASE,
    TK_DEFAULT,
    TK_FOR,
    TK_FOREACH,
    TK_DO,
    TK_NULL,
    TK_TRUE,
    TK_FALSE,
    TK_RETURN,
    TK_PUBLIC,
    TK_PROTECTED,
    TK_PRIVATE,
    TK_BREAK,
    TK_CONTINUE,
    TK_IN,
    TK_GOTO,
    TK_AS,
    TK_DOT,
    TK_OPENDIAMOND,
    TK_CLOSEDIAMOND,
    TK_OPENSQBRACKET,
    TK_CLOSESQBRACKET,
    TK_NAMESPACE,
    TK_USING,
    TK_TILDE,
    TK_ENUM,
    TK_UNION,
    TK_CAST,
    TK_HANDLE,
    TK_DIRECTIVE,
    TK_AUTO,
    TK_ASM,
    TK_INCREMENT,
    TK_DECREMENT,
    TK_ARROW,
    TK_LAMBDA, //< =>
    TK_IMPORT, //< import
    TK_NEWLINE,
    TK_UNKNOWN
};

struct token
{
   std::string text;
   token_t     type;
};

std::vector<token> tokenize(const std::string& sources);

}

#endif // RSLLEX_HPP_
