#include "IterativeOBJParser.h"

namespace Engine {
    std::vector<Vertex> IterativeOBJParser::ParseVertexes(std::ifstream &file, std::string &prefix) {
        std::string line;
        std::vector<Vertex> vertices;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            Vertex vertex{};
            std::string p;
            iss >> p;
            if (p == prefix) {
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
        }
        return vertices;
    }

    std::vector<TexCord> IterativeOBJParser::ParseTexCords(std::ifstream &file) {
        std::string line;
        std::vector<TexCord> tex_cords;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            TexCord cord{};
            std::string p;
            iss >> p;
            if (p == "vt") {
                iss >> cord.x >> cord.y;
                tex_cords.push_back(cord);
            }
        }
        return tex_cords;
    }

    std::vector<unsigned int> IterativeOBJParser::ParseFaces(std::ifstream &file) {
        std::vector<unsigned int> faces;
        std::string line;
        std::istringstream iss(line);
        std::string p;
        OBJFaceType fa = OBJFaceType::OnlyFaces;
        iss >> p;
        while (std::getline(file, line)) {
            if (p == "f") {
                if (line.find('/') == std::string::npos) {
                    unsigned int i;
                    iss >> i;
                    faces.push_back(i);
                    continue;
                }
                fa = OBJFaceType::AllIndices;
                line = line.substr(line.find(' ') + 1);
                std::string token = "0";
                for (int i = 0; line[i] != '\0'; i++) {
                    if (line[i] == '/' || line[i] == ' ') {
                        unsigned int j = std::stoi(token);
                        if (j != 0)
                            j--;
                        faces.push_back(j);
                        token = "0";
                        continue;
                    }
                    token += line[i];
                }
                unsigned int i = stoi(token);
                faces.push_back(i);
            }
        }
        faceType = fa;
        return faces;
    }


    void IterativeOBJParser::PrepData(const std::vector<Vertex> &_vertex, const std::vector<Vertex> &_vertexNormal,
                                           const std::vector<TexCord> &_vertexUv, const std::vector<unsigned int> &faces) {
        vertex = RearrangeVertices(_vertex, faces);
        if (!_vertexNormal.empty())
            vertexNormal = RearrangeVerticesNormals(_vertexNormal, faces);
        if (!_vertexUv.empty())
            vertexUv = RearrangeVerticesUvs(_vertexUv, faces);
    }

    std::vector<Vertex> IterativeOBJParser::RearrangeVertices(
        std::vector<Vertex> vertices, std::vector<unsigned int> faces){
        std::vector<Vertex> temp_vertices;
        if(faceType == OBJFaceType::AllIndices) {
            for (unsigned int i = 0; i < faces.size(); i += 3) {
                Vertex v = vertices.at(faces.at(i));
                temp_vertices.push_back(v);
            }
        }
        else if (faceType == OBJFaceType::OnlyFaces) {
            for (unsigned int i = 0; i < faces.size(); i++) {
                Vertex v = vertices.at(faces.at(i));
                temp_vertices.push_back(v);
            }
        }
        return temp_vertices;
    }

    std::vector<Vertex> IterativeOBJParser::RearrangeVerticesNormals(
    std::vector<Vertex> vertices, std::vector<unsigned int> faces) {
        std::vector<Vertex> temp_vertices;
        for (unsigned int i = 1; i < faces.size(); i += 3) {
            Vertex v = vertices.at(faces.at(i));
            temp_vertices.push_back(v);
        }
        return temp_vertices;
    }

    std::vector<TexCord> IterativeOBJParser::RearrangeVerticesUvs(
    std::vector<TexCord> vertices, std::vector<unsigned int> faces) {
        std::vector<TexCord> temp_cords;
        for (unsigned int i = 2; i < faces.size(); i += 3) {
            TexCord c = vertices.at(faces.at(i));
            temp_cords.push_back(c);
        }
        return temp_cords;
    }
}
