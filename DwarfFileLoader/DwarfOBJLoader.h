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


    struct SerializedFile {
        string fileName;
        string binPath;
    };
    class DwarfOBJLoader{

        DwarfOBJLoader() = default;
        ~DwarfOBJLoader() = default;
        static string defaultBinPath;

    public:
        static void GetBinaryFiles();

        static MeshData OBJFileParser(const string& filename);
        static vector<Vertex> GetVerticesFromData(MeshData& data);

        static void OBJDataSerializer(string& filepath, MeshData& meshData,
            const string& binPath = defaultBinPath);
        static MeshData OBJDataDeserializer(const string& filename);


        static vector<SerializedFile> FilesSerialized;

    };
}



#endif //DWARFOBJLOADER_H
