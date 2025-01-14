#include "DwarfIterativeOBJParser.h"

namespace Engine {
    vector<Vertex> DwarfIterativeOBJParser::ParseVertexes(std::ifstream &file, string &prefix) {
        string line;
        vector<Vertex> vertices;
        while (std::getline(file, line)) {
            istringstream iss(line);
            Vertex vertex{};
            string p;
            iss >> p;
            if (p == prefix) {
                iss >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
        }
        return vertices;
    }

    vector<TexCord> DwarfIterativeOBJParser::ParseTexCords(std::ifstream &file) {
        string line;
        vector<TexCord> tex_cords;
        while (std::getline(file, line)) {
            istringstream iss(line);
            TexCord cord{};
            string p;
            iss >> p;
            if (p == "vt") {
                iss >> cord.x >> cord.y;
                tex_cords.push_back(cord);
            }
        }
        return tex_cords;
    }

    vector<unsigned int> DwarfIterativeOBJParser::ParseFaces(std::ifstream &file) {
        vector<unsigned int> faces;
        string line;
        istringstream iss(line);
        string p;
        OBJFaceType fa = OBJFaceType::OnlyFaces;
        iss >> p;
        while (std::getline(file, line)) {
            if (p == "f") {
                if (line.find('/') == string::npos) {
                    unsigned int i;
                    iss >> i;
                    faces.push_back(i);
                    continue;
                }
                fa = OBJFaceType::AllIndices;
                line = line.substr(line.find(' ') + 1);
                string token = "0";
                for (int i = 0; line[i] != '\0'; i++) {
                    if (line[i] == '/' || line[i] == ' ') {
                        unsigned int j = stoi(token);
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


    void DwarfIterativeOBJParser::PrepData(const vector<Vertex> &_vertex, const vector<Vertex> &_vertexNormal,
                                           const vector<TexCord> &_vertexUv, const vector<unsigned int> &faces) {
        vertex = RearrangeVertices(_vertex, faces);
        if (!_vertexNormal.empty())
            vertexNormal = RearrangeVerticesNormals(_vertexNormal, faces);
        if (!_vertexUv.empty())
            vertexUv = RearrangeVerticesUvs(_vertexUv, faces);
    }

    vector<Vertex> DwarfIterativeOBJParser::RearrangeVertices(
        vector<Vertex> vertices, vector<unsigned int> faces){
        vector<Vertex> temp_vertices;
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

    vector<Vertex> DwarfIterativeOBJParser::RearrangeVerticesNormals(vector<Vertex> vertices, vector<unsigned int> faces) {
        vector<Vertex> temp_vertices;
        for (unsigned int i = 1; i < faces.size(); i += 3) {
            Vertex v = vertices.at(faces.at(i));
            temp_vertices.push_back(v);
        }
        return temp_vertices;
    }

    vector<TexCord> DwarfIterativeOBJParser::RearrangeVerticesUvs(vector<TexCord> vertices, vector<unsigned int> faces) {
        vector<TexCord> temp_cords;
        for (unsigned int i = 2; i < faces.size(); i += 3) {
            TexCord c = vertices.at(faces.at(i));
            temp_cords.push_back(c);
        }
        return temp_cords;
    }
}
