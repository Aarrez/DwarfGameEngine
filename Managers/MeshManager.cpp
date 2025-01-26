#include "MeshManager.h"

#include "ThreadManager.h"
#include "../FileLoader/OBJLoader.h"

namespace Engine {
    MeshManager* MeshManager::instance = nullptr;
    vector<Mesh> MeshManager::meshes = vector<Mesh>();

    void MeshManager::Allocate() {
        assert(instance == nullptr);
        if (instance != nullptr)
            return;

        instance = new MeshManager();
    }

    MeshManager* MeshManager::Instance() {
        return instance;
    }

    void MeshManager::ProcessMessage(MeshMessage *message) {
        auto msg = message->mMessage;
        switch (message->mType) {
            case MessageType::LoadMesh:
                ThreadManager::Instance()->QueueTask(&MeshManager::LoadMesh, msg, *this);
                break;
            case MessageType::AddMesh:
                ThreadManager::Instance()->QueueTask(&MeshManager::AddMesh, msg, *this);
                break;
            default:
                std::cerr <<
                    "Class can not process message of type: " << ToString(message->mType)<<
                        std::endl;
                break;
        }
    }


    Mesh MeshManager::LoadMesh(const std::string &fileName) {
        if (!meshes.empty())
            for (auto& mesh : meshes)
                if (fileName == mesh.name)
                    return mesh;

        for (auto& f : OBJLoader::FilesSerialized) {
            if (f.fileName == fileName) {
                auto mesh = OBJLoader::OBJDataDeserializer(fileName);
                mesh.name = fileName;
                meshes.push_back(mesh);
                return mesh;
            }
        }
        std::cerr << "File not found: " << fileName << std::endl;
        return Mesh();
    }

    Mesh MeshManager::AddMesh(const string& filePath) {
        std::cout << "Add Mesh: " << filePath << std::endl;

        return Mesh();
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
