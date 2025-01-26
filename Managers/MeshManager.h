#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "../Misc/PublicStructs.h"
#include "../GL/Mesh.h"
#include <iostream>

#include "../Message/MeshMessage.h"
#include "../Message/Message.h"

#include <thread>
#include <mutex>


namespace Engine {
    class MeshManager {

        static MeshManager* instance;
        vector<Mesh> meshes {};
        std::mutex addMeshMutex;
    public:
        static void Allocate();
        static MeshManager* Instance();
        void Processor(const string& filePath);
        void ProcessMessage(MeshMessage* message);
        Mesh LoadMesh(const string& fileName);
        Mesh AddMesh(const string& filePath);

        vector<Mesh>& GetMeshes();
        Mesh FindMesh(const string &meshName);
    };
}

#endif //MESHMANAGER_H
