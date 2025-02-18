#ifndef VirtualObject_h
#define VirtualObject_h
#include "../Stb/GetImage.h"
#include <vector>
#include "Shader.h"
#include "../FileLoader/OBJLoader.h"
#include "Mesh.h"

namespace Engine{
    class VirtualObject{
    public:

        VirtualObject(std::shared_ptr<Shader> _shader, const Mesh& mesh);

        void SetVertexBufferObjects(const Mesh& mesh);
        void BindLightVAO();
        void SetLightUniforms(std::shared_ptr<Shader> lightShader);
        void SetTextureUnit(unsigned int i);

        void Draw();

        ~VirtualObject();

    private:

        size_t vertices_size;

        size_t texture_count = 0;

        std::shared_ptr<Shader> shader;

        //Used for models and other stuff that is viable
        GLuint VBO{};
        GLuint NVBO{};
        GLuint TVBO{};

        GLuint lightVAO{};
        GLuint VAO{};

        GLuint texture1 {};

        size_t points_count{};

    };

}
#endif //VirtualObject_h