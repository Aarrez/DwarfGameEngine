#ifndef DWARFITOBJPARSER_H
#define DWARFITOBJPARSER_H

#include "../DwarfMisc/DwarfPublicStructs.h"
#include "fstream"
#include "sstream"
#include <vector>
#include <string>

#include "../DwarfMisc/PublicEnums.h"

namespace Engine {
    class DwarfIterativeOBJParser {

    public:
        DwarfIterativeOBJParser() = default;
        vector<Vertex> ParseVertexes(std::ifstream& file, string &prefix);
        vector<TexCord> ParseTexCords(std::ifstream& file);
        vector<unsigned int> ParseFaces(std::ifstream& file);

        void PrepData(
            const vector<Vertex> &vertex, const vector<Vertex> &vertexNormal,
            const vector<TexCord> &vertexUv, const vector<unsigned int> &faces);

        vector<Vertex> RearrangeVertices(vector<Vertex> vertices, vector<unsigned int> faces);
        vector<Vertex> RearrangeVerticesNormals(vector<Vertex> vertices, vector<unsigned int> faces);
        vector<TexCord> RearrangeVerticesUvs(vector<TexCord> vertices, vector<unsigned int> faces);

        vector<Vertex> vertex, vertexNormal;
        vector<TexCord> vertexUv;

    private:
        OBJFaceType faceType = OBJFaceType::OnlyFaces;


    };
}

#endif //DWARFITOBJPARSER_H
