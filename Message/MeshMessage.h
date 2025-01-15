#ifndef MESHMESSAGE_H
#define MESHMESSAGE_H
#include "Message.h"

#include <utility>

namespace Engine {
    class MeshMessage : public Message{
    public:
        explicit MeshMessage(std::string meshName) : Message(MessageType::LoadMesh, meshName) {};

    };
}

#endif //MESHMESSAGE_H
