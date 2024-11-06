#pragma once

namespace Dwarf {
    struct Vertex {
        float x;
    };

    static Vertex TriangleVertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    static const Vertex QuadVertices[] = {
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

        void PrepareTriangle(Vertex* vertices, unsigned int numVertices);

        void LoadShader() const {
            glUseProgram(shaderProgram);
            glBindVertexArray(vertex_array_object);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        GLuint shaderProgram;
        GLuint vertex_buffer_object{};
        GLuint vertex_array_object{};
        GLuint EBO{};

    private:
    };
}






