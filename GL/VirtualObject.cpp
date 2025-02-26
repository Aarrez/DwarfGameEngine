#include "VirtualObject.h"


namespace Engine {
    VirtualObject::VirtualObject(std::shared_ptr<Shader> _shader,
        const Mesh& mesh)
        :vertices_size(mesh.vertices.size()), shader(std::move(_shader)){

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &NVBO);
        glGenBuffers(1, &TVBO);

        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        SetVertexBufferObjects(mesh);
    }

    void VirtualObject::SetVertexBufferObjects(const Mesh& mesh) {
        vertices_size = mesh.vertices.size();

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
        mesh.vertices.size() * sizeof(Vertex),
                 &mesh.vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        if (!mesh.normals.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, NVBO);
            glBufferData(GL_ARRAY_BUFFER,
                mesh.normals.size() * sizeof(Vertex),
                &mesh.normals[0], GL_STATIC_DRAW);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
        }
        if (!mesh.uvs.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, TVBO);
            glBufferData(GL_ARRAY_BUFFER,
                mesh.uvs.size() * sizeof(TexCord),
                &mesh.uvs[0], GL_STATIC_DRAW);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(2);
        }

        BindLightVAO();
    }

    void VirtualObject::BindLightVAO() {
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void VirtualObject::Draw(GLuint _VAO){
        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_size));
    }

    VirtualObject::~VirtualObject() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteVertexArrays(1, &lightVAO);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &NVBO);
        glDeleteBuffers(1, &TVBO);
    }

    void VirtualObject::SetTextureUnit(unsigned int i) {
        shader->UseShaderProgram();
        shader->SetInt("texture1", i);
    }
}


