#include "lex.hpp"

namespace rsl {

struct lex_options
{
    bool ignoreNewlines = true;
};

token get_tok(const lex_options& options, const std::string& program_source, int& i)
{
    // We are at program_source[i], let's get the next token

    token token;
    char LastChar = ' ';

    // If i is already at the end, we give back the end of file token, which finishes the stream
    int sourceEnd = program_source.size() - 1;
    if (i >= sourceEnd) {
        token.type = TK_EOF;
        token.text = "EOF";
        return token;
    }

    LastChar = program_source[++i];

    if (LastChar == EOF)
    {
        token.type = TK_EOF;
        token.text = "EOF";
        return token;
    }

    if (LastChar == '\0')
    {
        token.type = TK_EOF;
        token.text = "EOF";
        return token;
    }

    auto is_garbage = [&](char c)
    {
        if (!options.ignoreNewlines && c == '\n')
            return false;

        if (c == '\r')
            return true;

        if (isspace(LastChar))
            return true;

        return false;
    };

    // we have room to work, skip whitespace
    while (is_garbage(LastChar))
        LastChar = program_source[++i];

    if (!options.ignoreNewlines && LastChar == '\n') {
        // tokenize every line for optional ;

        //m_cursor.line += 1;
        token.text = "\n";
        token.type = TK_NEWLINE;
        return token;
    }

    // Check for // comments
    if (LastChar == '/') {
        if (program_source[i + 1] == '/') {
            token.text = "/";

            // we have a comment
            while ((LastChar = program_source[++i]) != '\n') {
                token.text += LastChar;
            }

            token.type = TK_LINECOMMENT;
            return token;
        }
        else if (program_source[i + 1] == '*') {
            i++;
            LastChar = program_source[++i];

            //Log("First char %c", LastChar);

            bool foundPair = false;
            while (!foundPair) {
                if (LastChar == '*' && program_source[i + 1] == '/') {
                    ++i;
                    token.type = TK_MULTICOMMENT;
                    return token;
                }

                LastChar = program_source[++i];
            }
        }
    }

    if (LastChar == '(') {
        token.text = "(";
        token.type = TK_OPENPAREN;
        return token;
    }

    if (LastChar == ')') {
        token.text = ")";
        token.type = TK_CLOSEPAREN;
        return token;
    }

    if (LastChar == ':') {
        token.text = ":";
        token.type = TK_COLON;
        return token;
    }

    if (LastChar == '.') {
        token.text = ".";
        token.type = TK_DOT;
        return token;
    }

    if (LastChar == '[') {
        token.text = "[";
        token.type = TK_OPENSQBRACKET;
        return token;
    }

    if (LastChar == ']') {
        token.text = "]";
        token.type = TK_CLOSESQBRACKET;
        return token;
    }

    if (LastChar == ',') {
        token.text = ",";
        token.type = TK_COMMA;
        return token;
    }

    if (LastChar == '~') {
        token.text = "~";
        token.type = TK_TILDE;
        return token;
    }

    if (LastChar == '{') {
        token.text = "{";
        token.type = TK_OPENBRACKET;
        return token;
    }

    if (LastChar == '}') {
        token.text = "}";
        token.type = TK_CLOSEBRACKET;
        return token;
    }

    if (LastChar == ';') {
        token.type = TK_SEMICOLON;
        token.text = ";";
        return token;
    }

    if (LastChar == '@') {
        token.type = TK_HANDLE;
        token.text = "@";
        return token;
    }

    if (LastChar == '#') {
        token.type = TK_DIRECTIVE;
        token.text = "#";
        return token;
    }

    if (LastChar == '-') {
        if (program_source[i + 1] == '>') {
            i++;
            token.type = TK_ARROW;
            token.text = "->";
            return token;
        }
    }

    if (LastChar == '+') {
        if (program_source[i + 1] == '+') {
            i++;
            token.type = TK_INCREMENT;
            token.text = "++";
            return token;
        }
    }

    if (LastChar == '-') {
        if (program_source[i + 1] == '-') {
            i++;
            token.type = TK_DECREMENT;
            token.text = "--";
            return token;
        }
    }

    if (LastChar == '=' && program_source[i + 1] == '>') {
        token.type = TK_LAMBDA;
        token.text = "=>";
        return token;
    }

    if (LastChar == '|' && program_source[i + 1] == '|') {
        token.type = TK_BINOP;
        token.text = "||";
        ++i;
        return token;
    }

    if (LastChar == '&' && program_source[i + 1] == '&') {
        token.type = TK_BINOP;
        token.text = "&&";
        ++i;
        return token;
    }

    if (LastChar == '>' && program_source[i + 1] == '=') {
        token.type = TK_BINOP;
        token.text = ">=";
        ++i;
        return token;
    }

    if (LastChar == '<' && program_source[i + 1] == '=') {
        token.type = TK_BINOP;
        token.text = "<=";
        ++i;
        return token;
    }

    // operators
    if (LastChar == '>' || LastChar == '<' || LastChar == '+' || LastChar == '-' || LastChar == '*' || LastChar == '/' || LastChar == '^' ||
            LastChar == '=' || LastChar == '%') {
        token.type = TK_BINOP;
        token.text += LastChar;
        return token;
    }

    // We're facing a string literal
    if (LastChar == '"') {
        token.type = TK_STRINGLITERAL;
        token.text += LastChar;
        while ((LastChar = program_source[++i]) != '"') {
            token.text += LastChar;
        }
        token.text += LastChar;
        //EscapeString(token.text);
        return token;
    }

    // We're looking at a number literal
    if (isdigit(LastChar)) {
        token.text += LastChar;
        while (isdigit((LastChar = program_source[++i]))) {
            token.text += LastChar;
        }
        // digits are over, it can still be a floating point literal
        if (LastChar == '.') {
            token.text += ".";

            // read the remaining digits for the floating point number
            while (isdigit((LastChar = program_source[++i]))) {
                token.text += LastChar;
            }
            if (LastChar == 'f') {
                token.type = TK_FLOATLITERAL;
            }
            else {
                token.type = TK_FLOATLITERAL;
            }
        }
        else {
            token.type = TK_INTLITERAL;
        }

        --i;

        return token;
    }

    // let's find identifier
    if (isalpha(LastChar)) {
        token.text += LastChar;
        //token.pos = i;

        while (isalpha((LastChar = program_source[++i])) || isdigit(LastChar) || LastChar == '_') {
            token.text += LastChar;
        }

        // backtrack i by 1, so the next character read is the relevant item
        i--;
        if (token.text == "class")
            token.type = TK_CLASS;
        else if (token.text == "struct")
            token.type = TK_STRUCT;
        else if (token.text == "null" || token.text == "nullptr")
            token.type = TK_NULL;
        else if (token.text == "fn")
            token.type = TK_FUNCTION;
        else if (token.text == "function")
            token.type = TK_FUNCTION;
        else if (token.text == "enum")
            token.type = TK_ENUM;
        else if (token.text == "union")
            token.type = TK_UNION;
        else if (token.text == "cast")
            token.type = TK_CAST;
        else if (token.text == "if")
            token.type = TK_IF;
        else if (token.text == "else")
            token.type = TK_ELSE;
        else if (token.text == "while")
            token.type = TK_WHILE;
        else if (token.text == "do")
            token.type = TK_DO;
        else if (token.text == "const")
            token.type = TK_CONST;
        else if (token.text == "new")
            token.type = TK_NEW;
        else if (token.text == "delete")
            token.type = TK_DELETE;
        else if (token.text == "sizeof")
            token.type = TK_SIZEOF;
        else if (token.text == "typeof")
            token.type = TK_TYPEOF;
        else if (token.text == "operator")
            token.type = TK_OPERATOR;
        else if (token.text == "for")
            token.type = TK_FOR;
        else if (token.text == "foreach")
            token.type = TK_FOREACH;
        else if (token.text == "asm")
            token.type = TK_ASM;
        else if (token.text == "auto")
            token.type = TK_AUTO;
        else if (token.text == "true")
            token.type = TK_TRUE;
        else if (token.text == "false")
            token.type = TK_FALSE;
        else if (token.text == "return")
            token.type = TK_RETURN;
        else if (token.text == "public")
            token.type = TK_PUBLIC;
        else if (token.text == "protected")
            token.type = TK_PROTECTED;
        else if (token.text == "private")
            token.type = TK_PRIVATE;
        else if (token.text == "namespace")
            token.type = TK_NAMESPACE;
        else if (token.text == "using")
            token.type = TK_USING;
        else if (token.text == "break")
            token.type = TK_BREAK;
        else if (token.text == "goto")
            token.type = TK_GOTO;
        else if (token.text == "continue")
            token.type = TK_CONTINUE;
        else if (token.text == "as")
            token.type = TK_AS;
        else if (token.text == "in")
            token.type = TK_IN;
        else if (token.text == "switch")
            token.type = TK_SWITCH;
        else if (token.text == "case")
            token.type = TK_CASE;
        else if (token.text == "default")
            token.type = TK_DEFAULT;
        else if (token.text == "static")
            token.type = TK_STATIC;
        else if (token.text == "typedef")
            token.type = TK_TYPEDEF;
        else if (token.text == "and")
            token.type = TK_AND;
        else if (token.text == "is")
            token.type = TK_IS;
        else if (token.text == "isnt")
            token.type = TK_ISNOT;
        else if (token.text == "not")
            token.type = TK_NOT;
        else if (token.text == "import")
            token.type = TK_IMPORT;
        else
            token.type = TK_IDENTIFIER;
        return token;
    }
    return token;
}

std::vector<token> tokenize(const std::string& sources)
{
    if (sources.empty())
        return {};

    std::vector<token> tokens;

    int i = -1;

    lex_options options;
    options.ignoreNewlines = true;

    token t = get_tok(options, sources, i);
    while (t.type != TK_EOF) {
        tokens.push_back(t);
        t = get_tok(options, sources, i);
    }
    tokens.push_back(t); // push the eof

    return tokens;
}


}
