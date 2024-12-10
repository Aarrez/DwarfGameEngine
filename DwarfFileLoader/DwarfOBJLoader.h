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
#include "../DwarfMisc/DwarfPublicStructs.h"
using namespace std;

namespace Dwarf {

    struct MeshDataSize {
        size_t verticesCount, vFacesCount;
        size_t texCordCount, vNormalCount;
        size_t uvsCount, iNormalCount;
    };

    struct SerializedFile {
        string fileName;
        string path;
        MeshDataSize meshDataSize;;
    };
    class DwarfOBJLoader{

        DwarfOBJLoader() = default;
        ~DwarfOBJLoader();

    public:
        static MeshData OBJFileParser(const string& filename);
        static vector<Vertex> GetVerticesFromData(MeshData& data);

        static void OBJDataSerializer(string& filepath, MeshData& meshData,
            const string& binPath = "DwarfModels/BinaryFiles/");
        static MeshData OBJDataDeserializer(const char* filename);

        static vector<SerializedFile> FilesSerialized;

    };
}



#endif //DWARFOBJLOADER_H
