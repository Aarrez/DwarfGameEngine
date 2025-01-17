#ifndef MESHMESSAGE_H
#define MESHMESSAGE_H
#include "Message.h"

#include <utility>

namespace Engine {
    class MeshMessage : public Message{
    public:
        explicit MeshMessage(MessageType type ,std::string meshName) : Message(type, meshName) {};

    };
}

#endif //MESHMESSAGE_H
