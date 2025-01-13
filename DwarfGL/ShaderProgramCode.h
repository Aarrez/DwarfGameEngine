#pragma once

#include <string>
#include <fstream>
#include "glad/glad.h"
#include <filesystem>
#include "../DwarfMisc/DwarfPath.h"

namespace fs = std::filesystem;

namespace Dwarf {

    class DwarfGetShader {
    public:
        static GLuint LoadVertexShader(const char* path);

        static GLuint LoadFragmentShader(const char* path);
    private:

        static std::string ReadShaderFile(const char *pathToFile);

    };
}

