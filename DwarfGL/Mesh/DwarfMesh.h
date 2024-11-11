#pragma once
#include <memory>
#include <glad/glad.h>

#include "../DwarfShader.h"


namespace Dwarf::Mesh {
    class DwarfMesh2D{
    public:
        DwarfMesh2D(const VerticesStruct vertices[], size_t vertex_count);

        void Draw(DwarfShader* dwarf_shader);

        GLuint vertex_buffer_object{};
        GLuint vertex_array_object{};

        ~DwarfMesh2D();

    };

    class DwarfMesh {
    public:
        DwarfMesh(const VerticesStruct vertices[], size_t vertex_count,
            const unsigned int indices[]);

        void Draw(DwarfShader* dwarf_shader);

        GLuint vertex_buffer_object{};
        GLuint vertex_array_object{};
        GLuint element_buffer_object{};

        private:
        ~DwarfMesh();

    };
}


