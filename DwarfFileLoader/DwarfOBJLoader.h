//
// Created by Aaron on 2024-11-26.
//

#ifndef DWARFOBJLOADER_H
#define DWARFOBJLOADER_H
#include "iostream"
#include "string"
#include "vector"
#include <fstream>
#include <optional>
#include <sstream>

#include "../DwarfMisc/DwarfPublicStructs.h"
#include "../DwarfMisc/PublicEnums.h"
using namespace std;

namespace Dwarf {



    class DwarfOBJLoader{

        DwarfOBJLoader() = default;
        ~DwarfOBJLoader() = default;
        static string defaultBinPath;

    public:
        static void GetBinaryFiles();

        static std::optional<MeshData> OBJFileParser(const string& filename);
        static vector<Vertex> GetVerticesFromData(MeshData& data);
        static vector<Vertex> GetNormalsFromData(MeshData& data);
        static vector<TexCord> GetTexCoordFromData(MeshData& data);

        static void OBJDataSerializer(string& filepath, MeshData& meshData,
            const string& binPath = defaultBinPath);
        static MeshData OBJDataDeserializer(const string& filename);


        static vector<SerializedFile> FilesSerialized;

    };
}



#endif //DWARFOBJLOADER_H
