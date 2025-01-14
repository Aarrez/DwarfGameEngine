#include "MeshManager.h"

#include "../DwarfFileLoader/OBJLoader.h"

namespace Engine {
    MeshManager* MeshManager::instance = nullptr;

    void MeshManager::Allocate() {
        assert(instance == nullptr);
        if (instance != nullptr)
            return;

        instance = new MeshManager();
    }

    MeshManager* MeshManager::Instance() {
        return instance;
    }

    Mesh MeshManager::LoadMesh(const std::string &fileName) {
        auto mesh = OBJLoader::OBJDataDeserializer(fileName);
        mesh.name = fileName;
        meshes.push_back(mesh);
        return mesh;
    }

    Mesh MeshManager::AddMesh(const string& filePath) {
        auto meshData = OBJLoader::OBJFileParser(filePath);
        auto mesh = OBJLoader::OrderMeshData(meshData.value());
        auto file = OBJLoader::OBJDataSerializer(mesh, filePath);
        mesh.name = file.fileName;
        meshes.push_back(mesh);
        return mesh;
    }

    vector<Mesh> &MeshManager::GetMeshes() {
        return meshes;
    }

    Mesh* MeshManager::FindMesh(const string &meshName) {
        for (auto& mesh : meshes) {
            if (mesh.name == meshName) {
                return &mesh;
            }
        }
        return nullptr;
    }
}
