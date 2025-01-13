#include "VirtualObject.h"

#include <utility>

#include "DwarfMesh.h"

namespace Dwarf {
    VirtualObject::VirtualObject(std::shared_ptr<DwarfShader> _shader,
        vector<Vertex> vertices,
        vector<Vertex> normals,
        vector<TexCord> tex_cords)
        :vertices_size(vertices.size()), shader(std::move(_shader)){

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &NVBO);
        glGenBuffers(1, &TVBO);

        glGenVertexArrays(1, &VAO);
        /*glGenVertexArrays(1, &lightVAO);*/

        glBindVertexArray(VAO);


        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        if (!normals.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, NVBO);
            glBufferData(GL_ARRAY_BUFFER,
                normals.size() * sizeof(Vertex),
                &normals[0], GL_STATIC_DRAW);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
        }
        if (!tex_cords.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, TVBO);
            glBufferData(GL_ARRAY_BUFFER,
                tex_cords.size() * sizeof(TexCord),
                &tex_cords[0], GL_STATIC_DRAW);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(2);
        }

        glBindVertexArray(0);


        CreateTextures(texture1, "container.jpg", GL_RGB);
        stbi_set_flip_vertically_on_load(true);
        CreateTextures(texture2, "awesomeface.png", GL_RGBA);

    }

    void VirtualObject::Draw(){
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, vertices_size);
        glBindVertexArray(0);
    }

    VirtualObject::~VirtualObject() {
        glDeleteVertexArrays(1, &VAO);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &NVBO);
        glDeleteBuffers(1, &TVBO);

        glDeleteBuffers(1, &element_buffer_object);
    }

    void VirtualObject::CreateTextures(GLuint &texture, const char *image_name, int color_format) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        auto data = DwarfImage::GetImage(image_name, width, height, nrChannels);
        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, color_format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cerr << "Failed to load texture" << std::endl;
        stbi_image_free(data);

    }

    void VirtualObject::SetTextureUnit() {
        shader->UseShaderProgram();
        shader->SetInt("texture1", 0);
        shader->SetInt("texture2", 1);
    }

    void VirtualObject::BindOnTextureUnit() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
    }
}


