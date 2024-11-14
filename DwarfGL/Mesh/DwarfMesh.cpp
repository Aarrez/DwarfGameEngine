#include <iostream>
#include "DwarfMesh.h"


using namespace Dwarf;


Mesh2D::DwarfMesh2D::DwarfMesh2D(
        DwarfShader* shader,
        float *vertices,
        GLsizeiptr v_size,
        unsigned int *_indices,
        GLsizeiptr i_size) : dwarfShader(shader){
    glGenBuffers(1, &vertex_buffer_object);
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &element_buffer_object);

    //Bind Buffers & Arrays
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, v_size, vertices, GL_STATIC_DRAW);

    if(_indices){

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     i_size, _indices, GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof (float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    CreateTextures(texture1, "container.jpg", GL_RGB);
    stbi_set_flip_vertically_on_load(true);
    CreateTextures(texture2, "awesomeface.png", GL_RGBA);

}

void Mesh2D::DwarfMesh2D::Draw(DwarfShader* dwarf_shader){

   /* float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue)/2.0f) + 0.5f;
    dwarf_shader->SetVector4("VertexColor", 0.0f, greenValue, 0.0f, 1,0f);*/

    glBindVertexArray(vertex_array_object);
    if(element_buffer_object > 0){
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

void Mesh2D::DwarfMesh2D::CreateTextures(GLuint &texture, const char *image_name, int color_format) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    auto data = Image::DwarfImage::GetImage(image_name, width, height, nrChannels);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cerr << "Failed to load texture" << std::endl;
    stbi_image_free(data);

}

void Mesh2D::DwarfMesh2D::SetTextureUnit() {
    dwarfShader->UseShaderProgram();
    dwarfShader->SetInt("texture1", 0);
    dwarfShader->SetInt("texture2", 1);
}

void Mesh2D::DwarfMesh2D::BindOnTextureUnit() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
}





