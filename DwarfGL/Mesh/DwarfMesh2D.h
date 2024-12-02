#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../DwarfStb/DwarfGetImage.h"
#include <vector>
#include "../DwarfShader.h"
#include "../../DwarfFileLoader/DwarfOBJLoader.h"

namespace Dwarf::Mesh2D{
    static float TriVertices[] = {
            //Triangle corner        Corner colors     ModelTexture Coords
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
    static unsigned int s_indices[] {
            0, 1, 3,
            1, 2, 3
    };


    static float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };


    class DwarfMesh2D{
    public:

        DwarfMesh2D(std::shared_ptr<DwarfShader> shader,
            vector<Vertex> vertices);

        void CreateTextures(GLuint &texture, const char* image_name, int color_format);
        void SetTextureUnit();
        void BindOnTextureUnit();

        size_t vertices_size;;

        size_t texture_count = 0;

        std::shared_ptr<DwarfShader> dwarfShader;

        GLuint vertex_buffer_object{};
        GLuint vertex_array_object{};
        GLuint element_buffer_object{};
        GLuint texture1 {};
        GLuint texture2 {};

        size_t points_count{};

        ~DwarfMesh2D();

        void Draw(std::shared_ptr<DwarfShader> dwarf_shader);
    };

    /*class Triangle : public Mesh2D::DwarfMesh2D {
    public:
        explicit Triangle(DwarfShader* shader) :
                DwarfMesh2D(shader, TriVertices, sizeof(TriVertices),
                            nullptr, 0){
        }
    };

    class Square : public Mesh2D::DwarfMesh2D{
    public:
        explicit Square(DwarfShader* shader) :
                DwarfMesh2D(shader, vertices, sizeof(vertices),
                            nullptr, 0){;
        }
    };*/
}
