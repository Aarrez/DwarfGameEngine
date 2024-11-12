#pragma once
#include <memory>
#include <glad/glad.h>
#include <vector>
#include <optional>
#include "../DwarfShader.h"


namespace Dwarf::Mesh2D {


    static float TriVertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    static float QuadVertices[] = {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
    };
    static unsigned int indices[] {
            0, 1, 3,
            1, 2, 3
    };

    class DwarfMesh2D{
    public:

        DwarfMesh2D(float* vertices,
                    GLsizeiptr v_size,
                    size_t vertex_count,
                    unsigned int* _indices,
                    GLsizeiptr i_size,
                    size_t vertex_points);

        GLuint vertex_buffer_object{};
        GLuint vertex_array_object{};
        GLuint element_buffer_object{};

        size_t points_count{};

        ~DwarfMesh2D();

        void Draw(DwarfShader* dwarf_shader);
    };

    class Triangle : public Mesh2D::DwarfMesh2D {
    public:
        Triangle(size_t vc, size_t vp) :
        DwarfMesh2D(
                TriVertices, sizeof(TriVertices),
                vc, nullptr,
                    0 , vp){
            points_count = vp;
        }
    };

    class Square : public Mesh2D::DwarfMesh2D{
    public:
        Square(size_t vc, size_t vp) :
                DwarfMesh2D(QuadVertices, sizeof(TriVertices), vc, indices,sizeof(indices), vp){
            points_count = vp;
        }
    };
}


