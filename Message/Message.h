
#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>

#include "../DwarfMisc/PublicEnums.h"

namespace Engine {
    class Message {
    public:
        Message(MessageType type, std::string message);
        virtual ~Message();
        MessageType mType;
        std::string mMessage;
    };

}

#endif //MESSAGE_H
