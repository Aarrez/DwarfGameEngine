//
// Created by Aaron on 2024-11-26.
//

#ifndef DWARFOBJLOADER_H
#define DWARFOBJLOADER_H
#include "iostream"
#include "string"
#include "vector"
#include <fstream>
#include <sstream>
using namespace std;

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
    using MeshData = std::tuple<vector<Vertex>, vector<Face>>;
    class DwarfOBJLoader{

        DwarfOBJLoader() = default;
        static std::tuple<vector<Vertex>, vector<Face>> OBJFileParser(const string& filename);
    public:

        static vector<Vertex> GetVerticesFromOBJ(const string& filename);

    };

}



#endif //DWARFOBJLOADER_H
