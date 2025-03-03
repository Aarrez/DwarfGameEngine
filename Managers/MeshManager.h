#ifndef MESHMANAGER_H
#define MESHMANAGER_H
#include "../Misc/PublicStructs.h"
#include "../GL/Mesh.h"
#include <iostream>

#include "../Message/MeshMessage.h"
#include "../Message/Message.h"

#include "../FileLoader/OBJLoader.h"
#include "../Misc/Memory.h"


namespace Engine {
    class MeshManager {

        static MeshManager* instance;
        static std::vector<Mesh> meshes;
        void LoadMesh(const std::string& fileName);
        void AddMesh(const std::string& filePath);

    public:
        static void Allocate();
        static MeshManager* Instance();
        static void ProcessMessage(MeshMessage& message);

        std::vector<Mesh>& GetMeshes();
        Mesh FindMesh(const std::string &meshName);
    };
}

#endif //MESHMANAGER_H
