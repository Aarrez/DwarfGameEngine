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
        static GLuint LoadVertexShader();

        static GLuint LoadFragmentShader();
    private:

        static std::string ReadShaderFile(const GLchar *pathToFile);

    };
}

