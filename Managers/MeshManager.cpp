#include "MeshManager.h"
#include "ThreadManager.h"


namespace Engine {
    MeshManager* MeshManager::instance = nullptr;
    std::vector<Mesh> MeshManager::meshes {};

    void MeshManager::Allocate() {
        assert(instance == nullptr);
        if (instance != nullptr)
            return;

        instance = new MeshManager();
    }

    MeshManager* MeshManager::Instance() {
        return instance;
    }

    void MeshManager::ProcessMessage(MeshMessage& message) {
        auto msg = message.mMessage;
        switch (message.mType) {
            case MessageType::LoadMesh:
                Memory::PrintMemory();
                ThreadManager::Instance()->QueueTask(&MeshManager::LoadMesh, msg, *instance);
                break;
            case MessageType::AddMesh:
                Memory::PrintMemory();
                ThreadManager::Instance()->QueueTask(&MeshManager::AddMesh, msg, *instance);
                break;
            default:
                std::cerr <<
                    "Class can not process message of type: " <<
                        ToString(message.mType) <<
                        std::endl;
                break;
        }
    }


    void MeshManager::LoadMesh(const std::string &fileName) {
        if (!meshes.empty())
            for (auto& mesh : meshes)
                if (fileName == mesh.name)
                    return;
        Memory::PrintMemory();

        for (auto& f : OBJLoader::FilesSerialized) {
            if (f.fileName == fileName) {
                auto mesh = OBJLoader::OBJDataDeserializer(f.fileName);
                mesh.name = fileName;
                meshes.push_back(mesh);
                return;
            }
        }
        std::cerr << "File not found: " << fileName << std::endl;
    }

    void MeshManager::AddMesh(const std::string& filePath) {
        auto meshData = OBJLoader::OBJFileParser(filePath);
        if (!meshData.has_value()) {
            std::cout << "Can't find file: " << filePath << std::endl;
            return;
        }
        auto mesh = OBJLoader::OrderMeshData(meshData.value());
        auto file = OBJLoader::OBJDataSerializer(mesh, filePath);
        mesh.name = file.fileName;
        meshes.push_back(mesh);
        Memory::PrintMemory();
    }

    std::vector<Mesh> &MeshManager::GetMeshes() {
        return meshes;
    }

    Mesh MeshManager::FindMesh(const std::string &meshName) {
        for (auto& mesh : meshes) {
            if (mesh.name == meshName) {
                return mesh;
            }
        }
        LoadMesh(meshName);
        for (auto& mesh : meshes) {
            if (mesh.name == meshName) {
                return mesh;
            }
        }
        std::cerr << "Mesh not found: " << meshName << std::endl;
        return {};
    }
}
