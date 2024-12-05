//
// Created by Aaron.Marklund on 2024-11-19.
//

#include "DwarfMesh2D.h"

#include <utility>

#include "DwarfMesh.h"

namespace Dwarf {
    DwarfMesh2D::DwarfMesh2D(
        std::shared_ptr<DwarfShader> shader,
        vector<Vertex> vertices)
        :dwarfShader(std::move(shader)), vertices_size(vertices.size()){

    glGenBuffers(1, &vertex_buffer_object);
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &element_buffer_object);

    //Bind Buffers & Arrays
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);


    /*if(!_faces.empty()){

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     faces_size * sizeof(Face), &_faces[0], GL_STATIC_DRAW);

    }*/

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    CreateTextures(texture1, "container.jpg", GL_RGB);
    stbi_set_flip_vertically_on_load(true);
    CreateTextures(texture2, "awesomeface.png", GL_RGBA);

}

    void DwarfMesh2D::Draw(std::shared_ptr<DwarfShader> dwarf_shader){
        glBindVertexArray(vertex_array_object);


        /*if(element_buffer_object != 0){
            glDrawElements(GL_TRIANGLES, faces_size, GL_UNSIGNED_INT, 0);
            return;
        }*/

        glDrawArrays(GL_TRIANGLES, 0, vertices_size);
        glBindVertexArray(0);
    }

    DwarfMesh2D::~DwarfMesh2D() {
        glDeleteVertexArrays(1, &vertex_array_object);
        glDeleteBuffers(1, &vertex_buffer_object);
        glDeleteBuffers(1, &element_buffer_object);
    }

    void DwarfMesh2D::CreateTextures(GLuint &texture, const char *image_name, int color_format) {
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

    void DwarfMesh2D::SetTextureUnit() {
        dwarfShader->UseShaderProgram();
        dwarfShader->SetInt("texture1", 0);
        dwarfShader->SetInt("texture2", 1);
    }

    void DwarfMesh2D::BindOnTextureUnit() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
    }
}


