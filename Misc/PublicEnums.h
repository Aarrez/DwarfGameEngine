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
    };
    inline const char* ToString(const MessageType& msg) {
        switch (msg) {
            case MessageType::String: return "String";
            case MessageType::Float: return "Float";
            case MessageType::LoadMesh: return "LoadMesh";
            case MessageType::AddMesh: return "AddMesh";
            default: return "Unknown";
        }
    }
}

#endif //PUBLICENUMS_H
