#ifndef DWARFITOBJPARSER_H
#define DWARFITOBJPARSER_H

#include "../DwarfMisc/DwarfPublicStructs.h"
#include "fstream"
#include "sstream"
#include <vector>
#include <string>

#include "../DwarfMisc/PublicEnums.h"

namespace Dwarf {
    class DwarfIterativeOBJParser {

    public:
        DwarfIterativeOBJParser() = default;
        vector<Vertex> ParseVertexes(std::ifstream& file, string &prefix);
        vector<TexCord> ParseTexCords(std::ifstream& file);
        vector<unsigned int> ParseFaces(std::ifstream& file, OBJFaceType* faceType);

        void PrepData(
            vector<Vertex> vertex, vector<Vertex> vertexNormal,
            vector<TexCord> vertexUv, vector<unsigned int> faces);

        vector<Vertex> vertex, vertexNormal;
        vector<TexCord> vertexUv;
    private:
        vector<Vertex> RearrangeVertices(vector<Vertex> vertices, vector<unsigned int> faces);
        vector<Vertex> RearrangeVerticesNormals(vector<Vertex> vertices, vector<unsigned int> faces);
        vector<TexCord> RearrangeVerticesUvs(vector<TexCord> vertices, vector<unsigned int> faces);

    };
}

#endif //DWARFITOBJPARSER_H
