#include "DwarfMesh.h"



Dwarf::Mesh::DwarfMesh2D::DwarfMesh2D(const VerticesStruct vertices[], size_t vertex_count) {
    glGenBuffers(1, &vertex_buffer_object);
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriVertices), TriVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Dwarf::Mesh::DwarfMesh2D::Draw(DwarfShader* dwarf_shader){
    glUseProgram(dwarf_shader->shaderProgram);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


Dwarf::Mesh::DwarfMesh2D::~DwarfMesh2D() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
}

Dwarf::Mesh::DwarfMesh::DwarfMesh(
    const VerticesStruct vertices[], size_t vertex_count, const unsigned int indices[]) {

    /*//Generate Buffers & Arrays
    glGenBuffers(1, &vertex_buffer_object);
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &element_buffer_object);
    //Bind Buffers & Arrays
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, vertex_count, GL_FLOAT, GL_FALSE,
        vertex_count * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/
}

void Dwarf::Mesh::DwarfMesh::Draw(DwarfShader* dwarf_shader) {
    glUseProgram(dwarf_shader->shaderProgram);
    glBindVertexArray(vertex_array_object);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Dwarf::Mesh::DwarfMesh::~DwarfMesh() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
    glDeleteBuffers(1, &element_buffer_object);

}






