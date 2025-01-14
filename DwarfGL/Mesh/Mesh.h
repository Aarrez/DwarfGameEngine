#pragma once

#include "../DwarfMisc/DwarfPublicStructs.h"

namespace Engine {
    struct Face;
    struct TexCord;
    struct Vertex;

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

    struct Mesh : public IWriteAble {
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<Vertex> normals;
        std::vector<TexCord> uvs;

        bool WriteTo(std::ofstream &file) const override;
        bool ReadFrom(std::ifstream &file) override;
    };


}


