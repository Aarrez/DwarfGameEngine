#ifndef DWARFPUBLICSTRUCTS_H
#define DWARFPUBLICSTRUCTS_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <functional>
#include <string>
#include <variant>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "PublicEnums.h"


namespace Engine {
    //std::variant with the types int, float, string, const string&
    using Param = std::variant<int, float, std::string>;

    struct Task {
        TaskType type;
        std::function<void(Param)> task;
        Param arguments;
    };

    struct SerializedFile {
        std::string fileName;
        std::string binPath;
        bool selected;
    };

    enum class TextureWrapping {
        Repeat = GL_REPEAT,
        MirroredRepeat = GL_MIRRORED_REPEAT,
        ClampToEdge = GL_CLAMP_TO_EDGE,
        ClampToBorder = GL_CLAMP_TO_BORDER,
    };

    inline const char* ToCString(const TextureWrapping& wrapping){
        switch (wrapping) {
            case TextureWrapping::Repeat: return "GL_REPEAT";
            case TextureWrapping::MirroredRepeat: return "GL_MIRRORED_REPEAT";
            case TextureWrapping::ClampToEdge: return "GL_CLAMP_TO_EDGE";
            case TextureWrapping::ClampToBorder: return "GL_CLAMP_TO_BORDER";
            default: return "Unknown";
        }
    }

    enum class TextureFilter {
        Nearest = GL_NEAREST,
        Linear = GL_LINEAR,
        NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
        LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
        NearestMipmapLinear= GL_NEAREST_MIPMAP_LINEAR,
        LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    };

    inline const char* ToCString(const TextureFilter& filtering) {
        switch (filtering) {
            case TextureFilter::Nearest: return "GL_NEAREST";
            case TextureFilter::Linear: return "GL_LINEAR";
            case TextureFilter::NearestMipmapNearest: return "GL_NEAREST_MIPMAP_NEAREST";
            case TextureFilter::LinearMipmapLinear: return "GL_LINEAR_MIPMAP_LINEAR";
            case TextureFilter::NearestMipmapLinear: return "GL_NEAREST_MIPMAP_LINEAR";
            case TextureFilter::LinearMipmapNearest: return "GL_LINEAR_MIPMAP_NEAREST";
            default: return "Unknown";
        }
    }

    struct ShadowMap {
        unsigned int depthMapFBO;
        unsigned int depthMap;

        const unsigned int ShadowWidth = 1024;
        const unsigned int ShadowHeight = 1024;

    };

    struct Texture {
        std::string fileName;
        std::string filePath;
        unsigned int textureID;
        int colorFormat;

    private:
        TextureWrapping wrapping = TextureWrapping::Repeat;
        TextureFilter filtering_min = TextureFilter::LinearMipmapLinear;
        TextureFilter filtering_max = TextureFilter::LinearMipmapLinear;

    public:
        TextureWrapping GetWrapping() const {
            return wrapping;
        }
        TextureFilter GetFilteringMin() const {
            return filtering_min;
        }
        TextureFilter GetFilteringMax() const {
            return filtering_max;
        }

        void SetWrapping(TextureWrapping wrapping) {
            this->wrapping = wrapping;
        }
        void SetFilteringMin(TextureFilter min) {
            this->filtering_min = min;
        }
        void SetFilteringMax(TextureFilter max) {
            this->filtering_max = max;
        }
    };

    struct Entity {
        int id;
        std::string name;
        std::string meshName;
        Texture texture;
        Texture spec_texture;
        float shininess;
        glm::mat4 transform;
        bool selected;

    private:
        glm::mat4 PositionMatrix {glm::mat4(1.0f)};
        glm::mat4 scaleMatrix {glm::mat4(1.0f)};
        glm::mat4 rotateMatrix {glm::mat4(1.0f)};
        glm::vec3 rotRadians {};

    public:
        void Translate(glm::vec3 translation) {
            PositionMatrix = glm::translate(PositionMatrix, translation);
        }
        void SetPostion(glm::vec3 position) {
            PositionMatrix = glm::translate(glm::mat4(1.0f), position);
        }
        void SetScale(glm::vec3 scale) {
            scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        }
        void SetRotation(glm::vec3& rotation) {
            rotRadians = rotation;
            auto rotQuat = glm::quat(glm::radians(rotation));
            rotateMatrix = glm::toMat4(rotQuat);
        }
        void CombineTransformMatrix() {
            transform = PositionMatrix * rotateMatrix * scaleMatrix;
        }
        glm::vec3 GetPosition() {
            glm::vec3 pos = transform[3];
            return pos;
        }
        glm::vec3 GetScale() {
            glm::vec3 scale;
            scale.x = glm::length(glm::vec3(transform[0]));
            scale.y = glm::length(glm::vec3(transform[1]));
            scale.z = glm::length(glm::vec3(transform[2]));
            return scale;
        }
        glm::vec3 GetRotation() {
            return rotRadians;
        }
    };

    struct Vertex {
        float x, y, z;
    };
    struct Face {
        unsigned int x, y, z;
        void operator-= (const unsigned int &value) {
            x -= value;
            y -= value;
            z -= value;
        }
    };
    struct TexCord {
        float x, y;
    };




}

#endif //DWARFPUBLICSTRUCTS_H
