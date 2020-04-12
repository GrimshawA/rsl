#ifndef RSLGLSLTRANSPILER_HPP_
#define RSLGLSLTRANSPILER_HPP_

#include <string>
#include <vector>
#include "ast.hpp"

namespace rsl {

    class glsltranspiler
    {
    public:
        explicit glsltranspiler(const std::string& version, ast_module& module);

        void build();

    private:


    private:
        std::string version;
        ast_module& module;
    };
}

#endif // RSLBUILDER_HPP_
