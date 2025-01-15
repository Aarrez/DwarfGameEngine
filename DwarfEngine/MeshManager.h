#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "../DwarfMisc/DwarfPublicStructs.h"
#include "../DwarfGL/Mesh/Mesh.h"
#include <iostream>

#include "../Message/MeshMessage.h"
#include "../Message/Message.h"

#include <thread>
#include <mutex>


namespace Engine {
    class MeshManager {

        static MeshManager* instance;
        vector<Mesh> meshes;
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
