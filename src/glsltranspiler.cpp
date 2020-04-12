#include "glsltranspiler.hpp"

#include <stdio.h>
#include <sstream>

namespace rsl {

namespace glsl {


}

void write_expr(std::stringstream& ss, ast_expr* e, int nesting = 0)
{
    if (ast_is<ast_literal>(e)) {
        auto* expr = static_cast<ast_literal*>(e);
        ss << expr->value;
        return;
    }

    if (ast_is<ast_identifier>(e)) {
        auto* expr = static_cast<ast_identifier*>(e);
        ss << expr->name;
        return;
    }

    if (ast_is<ast_binary_op>(e)) {
        auto* expr = static_cast<ast_binary_op*>(e);

        if (nesting > 0)
            ss << "(";

        write_expr(ss, expr->lhs, nesting+1);
        ss << " " << expr->op << " ";
        write_expr(ss, expr->rhs, nesting+1);

        if (nesting > 0)
            ss << ")";
        return;
    }
}

glsltranspiler::glsltranspiler(const std::string& version, ast_module& module)
    : version(version)
    , module(module)
{

}

void glsltranspiler::build()
{
    std::stringstream ss;

    ss << "#version " << version << "\n\n";

    for (auto& file : module.files)
    {
        for (auto& decl : file.decls) {
            if (decl.type.hasQualifier(ast_type::In))
            {
                ss << "in " << decl.type.name << " " << decl.name << ";\n";
            }

            if (decl.type.hasQualifier(ast_type::Out))
            {
                ss << "out " << decl.type.name << " " <<decl.name << ";\n";
            }

            if (decl.type.hasQualifier(ast_type::Uniform))
            {
                ss << "uniform " << decl.type.name << " " <<decl.name << ";\n";
            }
        }

        ss << "\n";

        for (auto& func : file.functions)
        {
            ss << "void main() {\n";

            for (auto& stmt : func.block->stmts) {

                if (ast_is<ast_if>(stmt))
                {
                    ss << "if()\n";
                }

                if (ast_is<ast_expr>(stmt))
                {
                    write_expr(ss, static_cast<ast_expr*>(stmt));
                    ss << ";\n";
                }

                if (ast_is<ast_foreach>(stmt))
                {
                    ast_foreach* loop = static_cast<ast_foreach*>(stmt);
                    ss << "for (int _i = 0; i < " << loop->source.name << ") {\n";

                    ss << "}\n";
                }
            }

            ss << "}\n";
        }
    }

    FILE* f = fopen("output.glsl", "w");
    fwrite(ss.str().c_str(), ss.str().size(), 1, f);
    fclose(f);
}

}
