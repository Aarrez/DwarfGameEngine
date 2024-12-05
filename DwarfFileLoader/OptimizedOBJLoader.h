#ifndef OPTIMIZEDOBJLOADER_H
#define OPTIMIZEDOBJLOADER_H
#include "../DwarfMisc/DwarfPublicStructs.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace Dwarf {
    class OptimizedOBJLoader {
        OptimizedOBJLoader() = default;
    public:
        static void OptimizedOBJSerializer(const char* filepath, MeshData& meshData);
        static void OptimizedOBJDeserializer(const char* filePath);
        private:
        static int numVertices;
        static int numUVs;
        static int numNormals;
        static int numFaces;
    };
}



#endif //OPTIMIZEDOBJLOADER_H
