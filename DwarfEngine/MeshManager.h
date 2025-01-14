#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "../DwarfMisc/DwarfPublicStructs.h"
#include "../DwarfGL/Mesh/Mesh.h"
#include <iostream>


namespace Engine {
    class MeshManager {

        static MeshManager* instance;
        vector<Mesh> meshes;
    public:
        static void Allocate();
        static MeshManager* Instance();
        Mesh LoadMesh(const string& fileName);
        Mesh AddMesh(const string& filePath);
        vector<Mesh>& GetMeshes();
        Mesh* FindMesh(const string &meshName);
    };
}

#endif //MESHMANAGER_H
