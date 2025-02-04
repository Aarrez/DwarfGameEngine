#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>

#include "../Misc/PublicEnums.h"
#include <utility>

namespace Engine {
    class Message {
    public:
        Message(MessageType type, std::string message) : mType(type), mMessage(std::move(message)) {

        };
        virtual ~Message() = default;
        MessageType mType;
        std::string mMessage;
    };

}

#endif //MESSAGE_H
