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
    };

    class DwarfOBJLoader{
        DwarfOBJLoader();
    public:
        static std::tuple<vector<Vertex>, vector<Face>> DwarfVerticesParser(string filename);

    };

}



#endif //DWARFOBJLOADER_H
