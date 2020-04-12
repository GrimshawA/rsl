#include "utils.hpp"
#include <stdio.h>

namespace rsl {

std::string readFile(const std::string& filename)
{
    FILE* f = fopen(filename.c_str(), "r");

    if (!f)
        return {};

    fseek(f, 0, SEEK_END);
    auto size = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::string buff;
    buff.resize(size);
    fread((void*)buff.data(), size, 1, f);

    fclose(f);
    return buff;
}

}

