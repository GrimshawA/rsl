#include "builder.hpp"
#include "utils.hpp"
#include "lex.hpp"
#include "parse.hpp"
#include "glsltranspiler.hpp"

#include <iostream>

namespace rsl {

void builder::addFile(const std::string& filename)
{
    files.push_back(filename);
}

void builder::buildGlsl(const std::string& version)
{
    ast_module module;

    for (auto& file : files)
    {
        auto fileNode = build (file);

        // dummy stuff

//        auto func = ast_function{};
//        func.stmts.push_back(new ast_if());

//        fileNode.functions.push_back(func);
//        fileNode.functions.push_back({});


        module.files.push_back(fileNode);
    }

    glsltranspiler transpiler(version, module);
    transpiler.build();
}

ast_file builder::build(const std::string& filename)
{
    ast_file file;
    file.filename = filename;

    auto sources = readFile(filename);
    auto tokens = tokenize(sources);
    file = parse(tokens);


    std::cout << "source " << sources << std::endl;

    return file;
}

}
