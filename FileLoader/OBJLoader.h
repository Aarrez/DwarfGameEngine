#ifndef DWARFOBJLOADER_H
#define DWARFOBJLOADER_H
#include "iostream"
#include "string"
#include "vector"
#include <fstream>
#include <optional>
#include <sstream>

#include "../Misc/PublicStructs.h"
#include "../GL/Mesh.h"
#include "../Misc/PublicEnums.h"
using namespace std;

namespace Engine {
    class OBJLoader{

        OBJLoader() = default;
        ~OBJLoader() = default;
        static string defaultBinPath;


    public:
        static void GetBinaryFiles();

        static vector<Vertex> GetVerticesFromData(MeshData& data);
        static vector<Vertex> GetNormalsFromData(MeshData& data);
        static vector<TexCord> GetTexCoordFromData(MeshData& data);

        static std::optional<MeshData> OBJFileParser(const string& filename);
        static Mesh OrderMeshData(MeshData& data);

        static SerializedFile OBJDataSerializer(Mesh& mesh,const string& filePath,
            const string& binPath = defaultBinPath);
        static Mesh OBJDataDeserializer(const string& filename);


        static vector<SerializedFile> FilesSerialized;

    };
}



#endif //DWARFOBJLOADER_H
