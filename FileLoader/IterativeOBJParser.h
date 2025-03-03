#ifndef DWARFITOBJPARSER_H
#define DWARFITOBJPARSER_H

#include "../Misc/PublicStructs.h"
#include "fstream"
#include "sstream"
#include <vector>
#include <string>

#include "../Misc/PublicEnums.h"

namespace Engine {
    class IterativeOBJParser {

    public:
        IterativeOBJParser() = default;
        std::vector<Vertex> ParseVertexes(std::ifstream& file, std::string &prefix);
        std::vector<TexCord> ParseTexCords(std::ifstream& file);
        std::vector<unsigned int> ParseFaces(std::ifstream& file);

        void PrepData(
            const std::vector<Vertex> &vertex, const std::vector<Vertex> &vertexNormal,
            const std::vector<TexCord> &vertexUv, const std::vector<unsigned int> &faces);

        std::vector<Vertex> RearrangeVertices(
            std::vector<Vertex> vertices, std::vector<unsigned int> faces);
        std::vector<Vertex> RearrangeVerticesNormals(
            std::vector<Vertex> vertices, std::vector<unsigned int> faces);
        std::vector<TexCord> RearrangeVerticesUvs(
            std::vector<TexCord> vertices, std::vector<unsigned int> faces);

        std::vector<Vertex> vertex, vertexNormal;
        std::vector<TexCord> vertexUv;

    private:
        OBJFaceType faceType = OBJFaceType::OnlyFaces;


    };
}

#endif //DWARFITOBJPARSER_H
