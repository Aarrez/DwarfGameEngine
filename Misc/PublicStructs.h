#ifndef DWARFPUBLICSTRUCTS_H
#define DWARFPUBLICSTRUCTS_H
#define GLM_ENABLE_EXPERIMENTAL
#include <fstream>
#include <functional>
#include <string>
#include <variant>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "PublicEnums.h"
#include "../GL/Mesh.h"

using namespace std;

namespace Engine {
    //TODO add more allowed parameters
    using Param = variant<int, float, std::string>;

    struct Task {
        TaskType type;
        std::function<void(std::vector<Param>)> task;
        std::vector<Param> arguments;
    };

    struct SerializedFile {
        string fileName;
        string binPath;
        bool selected;
    };

    struct Texture {
        const char* filePath;
        int colorFormat;
    };

    struct Entity {
        int id;
        string name;
        string meshName;
        Texture texture;
        glm::mat4 transform;
        bool selected;

    private:
        glm::mat4 translateMatrix {glm::mat4(1.0f)};
        glm::mat4 scaleMatrix {glm::mat4(1.0f)};
        glm::mat4 rotateMatrix {glm::mat4(1.0f)};
        glm::vec3 rotRadians {};

    public:
        void Translate(glm::vec3 translation) {
            translateMatrix = glm::translate(glm::mat4(1.0f), translation);
        }
        void SetScale(glm::vec3 scale) {
            scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        }
        void SetRotation(glm::vec3& rotation) {
            rotRadians = rotation;
            auto rotQuat = glm::quat(glm::radians(rotation));
            rotateMatrix = glm::toMat4(rotQuat);
        }
        void SetTransformMatrix() {
            transform = translateMatrix * rotateMatrix * scaleMatrix;
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
