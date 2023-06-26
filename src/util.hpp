#pragma once

#include <fstream>
#include <sstream>
#include <string>

inline auto readFileBuf(const std::string& filename) -> std::string {
        std::ifstream readFile;
        readFile.open(filename);

        if(!readFile.is_open())
                return "";

        std::stringstream buffer;
        buffer << readFile.rdbuf();

        return buffer.str();
}

