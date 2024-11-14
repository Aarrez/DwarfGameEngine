#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../DwarfStb/DwarfGetImage.h"
#include <memory>
#include <vector>
#include <optional>
#include "../DwarfShader.h"



namespace Dwarf::Mesh2D {


    static float TriVertices[] = {
            //Triangle corner        Corner colors     Texture Coords
            -0.5f, -0.5f, 0.0f,     1.0f, .0f, .0f,     0.0f, 0.0f,
            0.5f, -0.5f, 0.0f,      .0f, 1.0f, .0f,     1.0f, 0.0f,
            0.0f,  0.5f, 0.0f,      .0f, .0f, 1.0f,     0.5f, 1.0f,
    };

    static float QuadVertices[] = {
            0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,    1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,
    };
    static unsigned int indices[] {
            0, 1, 3,
            1, 2, 3
    };

    static const float texCoords[]{
        .0f, .0f,
        1.0f, .0f,
        0.5f, 1.0f
    };

    class DwarfMesh2D{
    public:

        DwarfMesh2D(DwarfShader* shader,
                    float* vertices,
                    GLsizeiptr v_size,
                    unsigned int* _indices,
                    GLsizeiptr i_size);

        void CreateTextures(GLuint &texture, const char* image_name, int color_format);
        void SetTextureUnit();
        void BindOnTextureUnit();

        size_t texture_count = 0;

        DwarfShader* dwarfShader;

        GLuint vertex_buffer_object{};
        GLuint vertex_array_object{};
        GLuint element_buffer_object{};
        GLuint texture1 {};
        GLuint texture2 {};

        size_t points_count{};

        ~DwarfMesh2D();

        void Draw(DwarfShader* dwarf_shader);
    };

    class Triangle : public Mesh2D::DwarfMesh2D {
    public:
        explicit Triangle(DwarfShader* shader) :
        DwarfMesh2D(shader, TriVertices, sizeof(TriVertices),
                nullptr, 0){
        }
    };

    class Square : public Mesh2D::DwarfMesh2D{
    public:
        explicit Square(DwarfShader* shader) :
                DwarfMesh2D(shader, QuadVertices, sizeof(QuadVertices),
                            indices,sizeof(indices)){;
        }
    };
}


