#include "DwarfOBJLoader.h"

namespace Dwarf {
    std::tuple<vector<Vertex>, vector<Vertex>> DwarfOBJLoader::DwarfVerticesParser(string filename) {
        vector<Vertex> vertices;
        vector<Vertex> faces;
        std::ifstream file(filename);
        std::string line;
        if (!file.is_open()) {
            cerr << "File not found: " << filename << endl;
            return std::make_tuple(vertices, faces);
        }
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            string prefix;
            iss >> prefix;
            if (prefix == "v") {
                Vertex vertex;
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            if (prefix == "f") {
                Vertex face;
                iss >> face.x >> face.y >> face.z;
                faces.push_back(face);
            }
        }
        file.close();
        return std::make_tuple(vertices, faces);
    }

}

