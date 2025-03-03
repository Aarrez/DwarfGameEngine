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

namespace Engine {
    class OBJLoader{

        OBJLoader() = default;
        ~OBJLoader() = default;
        static std::string defaultBinPath;


    public:
        static void GetBinaryFiles();

        static std::vector<Vertex> GetVerticesFromData(MeshData& data);
        static std::vector<Vertex> GetNormalsFromData(MeshData& data);
        static std::vector<TexCord> GetTexCoordFromData(MeshData& data);

        static std::optional<MeshData> OBJFileParser(const std::string& filename);
        static Mesh OrderMeshData(MeshData& data);

        static SerializedFile OBJDataSerializer(Mesh& mesh,const std::string& filePath,
            const std::string& binPath = defaultBinPath);
        static Mesh OBJDataDeserializer(const std::string& filename);


        static std::vector<SerializedFile> FilesSerialized;

    };
}



#endif //DWARFOBJLOADER_H
