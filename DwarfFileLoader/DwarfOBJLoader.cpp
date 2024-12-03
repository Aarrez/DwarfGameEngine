#include "DwarfOBJLoader.h"

namespace Dwarf {
    std::tuple<vector<Vertex>, vector<Face>> DwarfOBJLoader::OBJFileParser(const string& filename) {
        vector<Vertex> vertices;
        vector<Face> faces;
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
                Vertex vert;
                iss >> vert.x >> vert.y >> vert.z;
                vertices.push_back(vert);
            }
            if (prefix == "f") {
                Face face;
                iss >> face.x >> face.y >> face.z;
                face -= 1;
                faces.push_back(face);
            }
        }
        file.close();
        return std::make_tuple(vertices, faces);
    }

    vector<Vertex> DwarfOBJLoader::GetVerticesFromOBJ(const std::string& filename) {
        MeshData data = OBJFileParser(filename);
        vector<Vertex> ordered_vertices;
        for (unsigned int i {0}; i < std::get<1>(data).size(); i++) {

            Face face = std::get<1>(data)[i];

            auto temp1 =
                std::get<0>(data)[face.x];
            auto temp2 =
                std::get<0>(data)[face.y];
            auto temp3 =
                std::get<0>(data)[face.z];

            ordered_vertices.push_back(temp1);
            ordered_vertices.push_back(temp2);
            ordered_vertices.push_back(temp3);
        }
        return ordered_vertices;
    }
}

