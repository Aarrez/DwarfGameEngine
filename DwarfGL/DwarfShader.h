#pragma once

namespace Dwarf {
    struct VerticesStruct {
        float vertices;
    };

    static const VerticesStruct TriVertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    static const VerticesStruct QuadVertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    static const unsigned int indeices[] {
        0, 1, 3,
        1, 2, 3
    };

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






