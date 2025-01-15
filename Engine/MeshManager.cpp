#include "MeshManager.h"

#include "../FileLoader/OBJLoader.h"

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

    void MeshManager::Processor(const string& filepath) {

    }


    void MeshManager::ProcessMessage(MeshMessage *message) {
        auto msg = message->mMessage;
        switch (message->mType) {
            case MessageType::LoadMesh:
                LoadMesh(msg);
                break;
            case MessageType::AddMesh:
                AddMesh(msg);
                break;
            default:
                std::cerr <<
                    "Class can not process message of type: " << ToString(message->mType)<<
                        std::endl;
                break;
        }
    }


    Mesh MeshManager::LoadMesh(const std::string &fileName) {
        for (auto& mesh : meshes) {
            if (fileName == mesh.name) {
                return mesh;
            }
        }
        auto mesh = OBJLoader::OBJDataDeserializer(fileName);
        mesh.name = fileName;
        meshes.push_back(mesh);
        return mesh;
    }

    Mesh MeshManager::AddMesh(const string& filePath) {
        //TODO start thread and then process the and obj file
        std::thread t1(&MeshManager::ProcessMessage, this, filePath);
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

    Mesh MeshManager::FindMesh(const string &meshName) {
        for (auto& mesh : meshes) {
            if (mesh.name == meshName) {
                return mesh;
            }
        }
        auto mesh = LoadMesh(meshName);
        return mesh;
    }
}
