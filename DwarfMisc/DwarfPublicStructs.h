#ifndef DWARFPUBLICSTRUCTS_H
#define DWARFPUBLICSTRUCTS_H
#include <vector>

namespace Dwarf {
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
        file.write(reinterpret_cast<char*>(vertexes.size()), sizeof(int));
        file.write(reinterpret_cast<char*>(texCords.size()), sizeof(int));
        file.write(reinterpret_cast<char*>(vertex_normals.size()), sizeof(int));
        file.write(reinterpret_cast<char*>(vertex_indexes.size()), sizeof(int));
        file.write(reinterpret_cast<char*>(uvs_indexes.size()), sizeof(int));
        file.write(reinterpret_cast<char*>(vertex_normals_indexes.size()), sizeof(int));
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
