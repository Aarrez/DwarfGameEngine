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
        vector<Vertex*> face;
    };

    struct Mesh {
        vector<Vertex*> vertices;
        vector<Face*> faces;
    };

    class DwarfOBJLoader{
        DwarfOBJLoader();
    public:
        static std::tuple<vector<Vertex>, vector<Vertex>> DwarfVerticesParser(string filename);

    };

}



#endif //DWARFOBJLOADER_H
