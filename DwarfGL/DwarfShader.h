#pragma once

namespace Dwarf {



    class DwarfShader {
    public:
        DwarfShader();
        ~DwarfShader();

        GLuint shaderProgram;
        //Shaders
        GLuint vertexShader{};
        GLuint fragmentShader{};

    private:
    };
}






