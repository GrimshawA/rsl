#ifndef RSLAST_HPP_
#define RSLAST_HPP_

#include <string>
#include <vector>

namespace rsl {

struct ast_node
{

};

struct ast_type
{
    enum qualifiers_t {
        In,
        Out,
        Uniform,
        Const
    };

    bool hasQualifier(qualifiers_t q)
    {
        for (auto& qq : qualifiers)
        {
            if (qq == q)
                return true;
        }

        return false;
    }

    std::string name;
    std::vector<qualifiers_t> qualifiers;
};

struct ast_stmt
{
    virtual ~ast_stmt() = default;
};

struct ast_expr : ast_stmt
{

};

struct ast_binary_op : ast_expr
{
    ast_expr* lhs = nullptr;
    ast_expr* rhs = nullptr;
    std::string op;
};

struct ast_block : ast_stmt
{
    std::vector<ast_stmt*> stmts;
};

struct ast_function
{
    std::string name;
    ast_block*  block = nullptr;
};

struct ast_operation : ast_stmt
{

};

struct ast_return : ast_stmt
{
    ast_expr* expr = nullptr;
};

struct ast_while
{

};

struct ast_for
{

};



struct ast_if : ast_stmt
{

};

struct ast_literal : ast_expr
{
    std::string value;
};

struct ast_identifier : ast_expr
{
    std::string name;
};

struct ast_foreach : ast_stmt
{
    ast_identifier iterator;
    ast_identifier source;
    ast_block* block = nullptr;
};

struct ast_decl
{
    std::string name;
    ast_type type;
};

struct ast_struct
{

};

struct ast_import
{
    std::string value;
};

struct ast_file
{
    std::string filename;
    std::vector<ast_import> imports;
    std::vector<ast_function> functions;
    std::vector<ast_decl> decls;
};

struct ast_module
{
    std::vector<ast_file> files;
};

//// manipulators
///

template<typename T, typename R>
bool ast_is(R* node)
{
    return dynamic_cast<T*>(node);
}

}

#endif // RSLAST_HPP_
