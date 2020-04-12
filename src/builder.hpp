#ifndef RSLBUILDER_HPP_
#define RSLBUILDER_HPP_

#include <string>
#include <vector>
#include "ast.hpp"

namespace rsl {

    class builder
    {
    public:
        void addFile(const std::string& filename);
        void buildGlsl(const std::string& version);

    private:
        ast_file build(const std::string& filename);

        std::vector<std::string> files;
    };
}

#endif // RSLBUILDER_HPP_
