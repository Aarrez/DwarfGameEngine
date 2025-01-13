#ifndef DWARFPUBLICSTRUCTS_H
#define DWARFPUBLICSTRUCTS_H
#define GLM_ENABLE_EXPERIMENTAL
#include <fstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;

namespace Dwarf {

    struct SerializedFile {
        string fileName;
        string binPath;
        bool selected;
    };

    struct Entity {
        int id;
        string name;
        SerializedFile fileInfo;
        string texture;
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
    class IWriteAble {
    public:
        virtual ~IWriteAble() = default;

        virtual bool WriteTo(std::ofstream& file) const = 0;
        virtual bool ReadFrom(std::ifstream& file) = 0;
    };

    struct MeshData : public IWriteAble{
        std::vector<Vertex> vertexes;
        std::vector<TexCord> texCords;
        std::vector<Vertex> vertex_normals;
        std::vector<Face> vertex_indexes;
        std::vector<Face> uvs_indexes;
        std::vector<Face> vertex_normals_indexes;

        bool WriteTo(std::ofstream &file) const override;
        bool ReadFrom(std::ifstream &file) override;

    };

    inline bool MeshData::WriteTo(std::ofstream &file) const {
        int v, vt, vn, f, uv, fn;
        v = vertexes.size();
        vt = texCords.size();
        vn = vertex_normals.size();
        f = vertex_indexes.size();
        uv = uvs_indexes.size();
        fn = vertex_normals_indexes.size();
        file.write(reinterpret_cast<const char*>(&v), sizeof(int));
        file.write(reinterpret_cast<const char*>(&vt), sizeof(int));
        file.write(reinterpret_cast<const char*>(&vn), sizeof(int));
        file.write(reinterpret_cast<const char*>(&f), sizeof(int));
        file.write(reinterpret_cast<const char*>(&uv), sizeof(int));
        file.write(reinterpret_cast<const char*>(&fn), sizeof(int));
        for (int i {0}; i < vertexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertexes[i]), sizeof(Vertex));
        }
        for (int i {0}; i < texCords.size(); i++) {
            file.write(reinterpret_cast<const char*>(&texCords[i]), sizeof(TexCord));
        }
        for (int i {0}; i < vertex_normals.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertex_normals[i]), sizeof(Vertex));
        }
        for (int i {0}; i < vertex_indexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertex_indexes[i]), sizeof(Face));
        }
        for (int i {0}; i < uvs_indexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&uvs_indexes[i]), sizeof(Face));
        }
        for (int i {0}; i < vertex_normals_indexes.size(); i++) {
            file.write(reinterpret_cast<const char*>(&vertex_normals_indexes[i]), sizeof(Face));
        }
        if (file.fail()) {
            return false;
        }
        return true;
    }

    inline bool MeshData::ReadFrom(std::ifstream &file) {
        vertexes.clear();
        texCords.clear();
        vertex_normals.clear();
        vertex_indexes.clear();
        uvs_indexes.clear();
        vertex_normals_indexes.clear();
        int v, vt, vn, f, uv, fn;
        file.read(reinterpret_cast<char*>(&v), sizeof(int));
        file.read(reinterpret_cast<char*>(&vt), sizeof(int));
        file.read(reinterpret_cast<char*>(&vn), sizeof(int));
        file.read(reinterpret_cast<char*>(&f), sizeof(int));
        file.read(reinterpret_cast<char*>(&uv), sizeof(int));
        file.read(reinterpret_cast<char*>(&fn), sizeof(int));
        for (int i {0}; i < v; i++) {
            Vertex temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            vertexes.push_back(temp);
        }
        for (int i {0}; i < vt; i++) {
            TexCord temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(TexCord));
            texCords.push_back(temp);
        }
        for (int i {0}; i < vn; i++) {
            Vertex temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Vertex));
            vertex_normals.push_back(temp);
        }
        for (int i {0}; i < f; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            vertex_indexes.push_back(temp);
        }
        for (int i {0}; i < uv; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            uvs_indexes.push_back(temp);
        }
        for (int i {0}; i < fn; i++) {
            Face temp {};
            file.read(reinterpret_cast<char*>(&temp), sizeof(Face));
            vertex_normals_indexes.push_back(temp);
        }
        if (file.fail()) {
            return false;
        }
        return true;
    }
}

#endif //DWARFPUBLICSTRUCTS_H
