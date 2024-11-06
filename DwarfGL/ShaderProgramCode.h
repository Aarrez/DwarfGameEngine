#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "glad/glad.h"

namespace Dwarf {

    class DwarfGetShader {
    public:
        static GLuint LoadVertexShader();

        static GLuint LoadFragmentShader();

    private:

        static std::string ReadShaderFile(const GLchar *pathToFile);

    };
}

