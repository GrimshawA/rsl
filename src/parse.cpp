#include "parse.hpp"

#include <map>

std::map<std::string, int> mOperatorTable = {
    { "=", 0 },
    { ">", 10 },
    { "<", 10 },
    { ">=", 10 },
    { "<=", 10 },
    { "==", 10 },
    { "!=", 10 },
    { "+", 20 },
    { "-", 20 },
    { "^", 30 },
    { "*", 30 },
    { "/", 30 },
    { "%", 30 },

    { "||", 10 },
    { "&&", 10 },

};

namespace rsl {

class parse_state;

ast_block* parseBlock(parse_state& state);



class parse_state
{
public:
    explicit parse_state(std::vector<token>& ss)
        : stream(ss)
        , curr(&stream.at(0))
    {

    }

    token_t currentType()
    {
        return curr->type;
    }

    token current()
    {
        return *curr;
    }

    token next()
    {
        ++curr;
        while (curr->type == TK_NEWLINE)
           ++curr;
        while (curr->type == TK_LINECOMMENT)
           ++curr;
        return *curr;
    }

    token next(int count)
    {
        for (int i = 0; i < count; ++i)
            next();
        return current();
    }

    std::string text()
    {
        return current().text;
    }

    token nextSkipComment()
    {
        while (next().type == TK_LINECOMMENT);
        return *curr;
    }

    std::vector<token>& stream;
    token* curr;
};

ast_expr* parseExpression(parse_state& state)
{
    ast_expr* expr = nullptr;
    std::vector<std::string> operations;
    std::vector<ast_expr*>   operands;

    bool end = false;

    while (!end)
    {
        if (state.currentType() == TK_IDENTIFIER)
        {
            auto* ident = new ast_identifier;
            ident->name = state.current().text;

            operands.push_back(ident);

            state.next();
        }
        else if (state.currentType() == TK_OPENPAREN)
        {
            state.next();
            auto* expr = parseExpression(state);
            operands.push_back(expr);
        }
        else if (state.currentType() == TK_BINOP)
        {
            operations.push_back(state.current().text);
            state.next();
        }
        else if (state.currentType() == TK_INTLITERAL)
        {
            ast_literal* lit = new ast_literal();
            lit->value = state.current().text;
            operands.push_back(lit);
            state.next();
        }
        else if (state.currentType() == TK_SEMICOLON)
        {
            state.next();
            end = true;
        }

        else if (state.currentType() == TK_CLOSEPAREN)
        {
            state.next();
            end = true;
        }
    }

    // Assemble expression trees
    if (operations.size() == 0 && operands.size() == 1)
    {
        return operands[0];
    }

    if (operations.size() == 1 && operands.size() == 2)
    {
        ast_binary_op* op = new ast_binary_op();
        op->lhs = operands[0];
        op->rhs = operands[1];
        op->op = operations[0];
        return op;
    }

    auto* cachedlhs = operands[0];
    for (int i = 0; i < operations.size() - 1; ++i)
    {
        bool rightassoc = mOperatorTable[operations[i]] > mOperatorTable[operations[i+1]];

        if (rightassoc)
        {
            ast_binary_op* op = new ast_binary_op();
            op->lhs = operands[i];
            op->rhs = operands[i+1];
            op->op = operations[i+1];
            cachedlhs = op;

            if (i == operations.size() - 1)
            {
                ast_binary_op* op = new ast_binary_op();
                op->lhs = cachedlhs;
                op->rhs = operands.back();
                op->op = operations[operations.size()-1];
                cachedlhs = op;
            }
        }
        else
        {
            ast_binary_op* rhs = new ast_binary_op();
            rhs->lhs = operands[i+1];
            rhs->rhs = operands[i+2];
            rhs->op = operations[i+1];

            ast_binary_op* lhs = new ast_binary_op;
            lhs->lhs = operands[i];
            lhs->rhs = rhs;
            lhs->op = operations[i];

            cachedlhs = lhs;
        }
    }

    return cachedlhs;
}

ast_type parseType(parse_state& state)
{
    ast_type type;

    while (state.currentType() != TK_SEMICOLON)
    {
        auto word = state.current().text;

        if (word == "in") {
            type.qualifiers.push_back(ast_type::In);
        }
        else if (word == "out") {
            type.qualifiers.push_back(ast_type::Out);
        }
        else if (word == "uniform") {
            type.qualifiers.push_back(ast_type::Uniform);
        }
        else {
            type.name = word;
        }

        state.next();
    }

    state.next();

    return type;
}

ast_foreach* parseForEach(parse_state& state)
{
    ast_foreach* loop = new ast_foreach();

    state.next();

    state.next();

    loop->iterator.name = state.current().text;

    state.next(); // in

    loop->source.name = state.next().text;

    state.next(); // )

    state.next();

    loop->block = parseBlock(state);

    return loop;
}

ast_block* parseBlock(parse_state& state)
{
    ast_block* blk = new ast_block;

    // Go into its body
    state.next();

    while (state.currentType() != TK_CLOSEBRACKET)
    {


        if (state.currentType() == TK_FOREACH)
        {
            ast_foreach* loop = parseForEach(state);
            blk->stmts.push_back(loop);
        }
        else if (state.currentType() == TK_RETURN)
        {
            ast_return* ret = new ast_return;

            state.next();

            ast_expr* expr = nullptr;
            if (state.currentType() != TK_SEMICOLON)
            {
                expr = parseExpression(state);
            }

            ret->expr = expr;
            blk->stmts.push_back(ret);
        }
        else
        {
            // If it is nothing else, its an expression!
            auto* expr = parseExpression(state);
            blk->stmts.push_back(expr);
        }
    }

    // Clear the }
    state.next();

    return blk;
}

ast_function parseFunction(parse_state& state)
{
    ast_function func;

    func.name = state.next().text;

    // After name, either return type or param list or block

    state.next();

    if (state.currentType() == TK_OPENBRACKET)
    {
        func.block = parseBlock(state);
    }

    return func;
}

void parseTopLevelDecl(parse_state& state, ast_file& file)
{
    auto name = state.current().text;

    // Should be a :
    state.next(2);


    ast_decl decl;
    decl.name = name;
    decl.type = parseType(state);
    file.decls.push_back(decl);
}

ast_import parseImport(parse_state& state)
{
    ast_import import;

    import.value = state.next().text;

    state.next(2);

    return import;
}

ast_file parse(std::vector<token>& tokens)
{
    if (tokens.empty())
        return {};

    ast_file file;

    parse_state state(tokens);

    while (state.currentType() != TK_EOF)
    {
        if (state.currentType() == TK_IMPORT)
        {
            file.imports.push_back(parseImport(state));
        }

        if (state.currentType() == TK_FUNCTION)
        {
            auto func = parseFunction(state);
            file.functions.push_back(func);
        }

        if (state.currentType() == TK_IDENTIFIER)
        {
            parseTopLevelDecl(state, file);
        }

    }

    return file;
}

}
