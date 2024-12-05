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

    struct MeshData {
        std::vector<Vertex> vertices;
        std::vector<TexCord> texCords;
        std::vector<Vertex> normals_vertices;
        std::vector<Face> vertices_faces;
        std::vector<Face> uvs_faces;
        std::vector<Face> normals_faces;

    };
}

#endif //DWARFPUBLICSTRUCTS_H
