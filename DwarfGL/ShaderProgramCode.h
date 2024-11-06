#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "glad/glad.h"

namespace Dwarf {

    class DwarfGetShader {
    public:
        static const GLchar* GetVertexShader();

        static const GLchar* GetFragmentShader();

    private:
        static std::string VertexShader;
        static std::string FragmentShader;

        static void readVertexFile(const GLchar *pathToFile);
        static void readFragmentFile(const GLchar *pathToFile);

    };
}

