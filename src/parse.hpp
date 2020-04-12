#ifndef RSLPARSE_HPP_
#define RSLPARSE_HPP_

#include "lex.hpp"
#include "ast.hpp"

namespace rsl {
    ast_file parse(std::vector<token>& tokens);
}

#endif // RSLPARSE_HPP_
