#ifndef MESHMANAGER_H
#define MESHMANAGER_H
#include "ThreadManager.h"
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
    public:
        static void Allocate();
        static MeshManager* Instance();
        void ProcessMessage(MeshMessage* message);
        Mesh LoadMesh(const string& fileName);
        Mesh AddMesh(const string& filePath);

        vector<Mesh>& GetMeshes();
        Mesh FindMesh(const string &meshName);
    };
}

#endif //MESHMANAGER_H
