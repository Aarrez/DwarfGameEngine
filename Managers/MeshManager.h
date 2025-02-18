#ifndef MESHMANAGER_H
#define MESHMANAGER_H
#include "../Misc/PublicStructs.h"
#include "../GL/Mesh.h"
#include <iostream>

#include "../Message/MeshMessage.h"
#include "../Message/Message.h"

#include "../FileLoader/OBJLoader.h"


namespace Engine {
    class MeshManager {

        static MeshManager* instance;
        static vector<Mesh> meshes;
        void LoadMesh(const string& fileName);
        void AddMesh(const string& filePath);

    public:
        static void Allocate();
        static MeshManager* Instance();
        static void ProcessMessage(MeshMessage& message);

        vector<Mesh>& GetMeshes();
        Mesh FindMesh(const string &meshName);
    };
}

#endif //MESHMANAGER_H
