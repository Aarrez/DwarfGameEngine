#ifndef PUBLICENUMS_H
#define PUBLICENUMS_H

namespace Engine {

    enum class TaskType {
        Execute = 0,
        Stop = 1,
    };
    enum class OBJLoadingState {
        OpenFile = 0,
        Vertex = 1,
        VertexNormal = 2,
        VertexUV = 3,
        Faces = 4,
        PrepData = 5,
    };

    enum class OBJFaceType {
        OnlyFaces = 0,
        AllIndices = 1,
    };

    enum class MessageType {
        String = 0,
        Float = 1,
        LoadMesh = 2,
        AddMesh = 3,
        CreateEntity = 4,
        RemoveEntityByName = 5,
        RemoveAllEntities = 6,
    };
    inline const char* ToString(const MessageType& msg) {
        switch (msg) {
            case MessageType::String: return "String";
            case MessageType::Float: return "Float";
            case MessageType::LoadMesh: return "LoadMesh";
            case MessageType::AddMesh: return "AddMesh";
            case MessageType::CreateEntity: return "AddEntity";
            case MessageType::RemoveEntityByName: return "RemoveEntityByName";
            default: return "Unknown";
        }
    }

    enum class CollisionTypes {
        NoCollision = 0,
        BoxCollision = 1,
        SphereCollision = 2,
        RayCollision = 3,
        RayABBBCollision = 4
    };

    inline const char* ToCString(const CollisionTypes& type) {
        switch (type) {
            case CollisionTypes::NoCollision: return "NoCollision";
            case CollisionTypes::SphereCollision: return "SphereCollision";
            case CollisionTypes::BoxCollision: return "BoxCollision";
            case CollisionTypes::RayCollision: return "RayCollision";
            case CollisionTypes::RayABBBCollision: return "RayABBBCollision";
            default: return "Unknown";

        }
    }
}

#endif //PUBLICENUMS_H
