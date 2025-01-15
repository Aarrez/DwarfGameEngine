#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../DwarfStb/DwarfGetImage.h"
#include <vector>
#include "../DwarfShader.h"
#include "../../DwarfFileLoader/OBJLoader.h"

namespace Engine{
    class VirtualObject{
    public:

        VirtualObject(std::shared_ptr<DwarfShader> _shader, const Mesh& mesh);

        void SetVertexBufferObjects(const Mesh& mesh);
        void CreateTextures(GLuint &textureID, const Texture &texture);
        void SetTexture(const Texture& texture);
        void SetTextureUnit();
        void BindOnTextureUnit();

        void Draw();

        ~VirtualObject();

    private:

        size_t vertices_size;

        size_t texture_count = 0;

        std::shared_ptr<DwarfShader> shader;

        //Used for models and other stuff that is viable
        GLuint VBO{};
        GLuint NVBO{};
        GLuint TVBO{};

        GLuint lightVAO{};
        GLuint VAO{};
        GLuint element_buffer_object{};

        GLuint texture1 {};
        GLuint texture2 {};

        size_t points_count{};

    };

}
