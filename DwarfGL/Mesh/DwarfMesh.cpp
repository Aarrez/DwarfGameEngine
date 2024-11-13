#include <iostream>
#include "DwarfMesh.h"

using namespace Dwarf;


Mesh2D::DwarfMesh2D::DwarfMesh2D(
        float *vertices,
        GLsizeiptr v_size,
        size_t vertex_count,
        unsigned int *_indices,
        GLsizeiptr i_size,
        size_t vertex_points) {
    glGenBuffers(1, &vertex_buffer_object);
    glGenVertexArrays(1, &vertex_array_object);

    //Bind Buffers & Arrays
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_STATIC_DRAW);

    if(_indices){
        glGenBuffers(1, &element_buffer_object);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     i_size, _indices, GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, vertex_count, GL_FLOAT, GL_FALSE,
                          vertex_points * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, vertex_count, GL_FLOAT, GL_FALSE,
                          vertex_points * sizeof(float), (void*)(vertex_count * sizeof(float)));
    glEnableVertexAttribArray(1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


    float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



}

void Mesh2D::DwarfMesh2D::Draw(DwarfShader* dwarf_shader){

   /* float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue)/2.0f) + 0.5f;
    dwarf_shader->SetVector4("VertexColor", 0.0f, greenValue, 0.0f, 1,0f);*/

    glBindVertexArray(vertex_array_object);
    if(element_buffer_object > 0){
        glDrawElements(GL_TRIANGLES, points_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        return;
    }


    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Mesh2D::DwarfMesh2D::~DwarfMesh2D() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
    glDeleteBuffers(1, &element_buffer_object);
}





