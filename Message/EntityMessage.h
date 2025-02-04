#ifndef ENTITYMESSAGE_H
#define ENTITYMESSAGE_H

#include "Message.h"
#include "../Misc/PublicStructs.h"

namespace Engine {
  class EntityMessage : public Message {
  public:
    EntityMessage(const MessageType type, const std::string &msg) : Message(type, msg) {};
    SerializedFile file;
    Texture texture;


  };
}



#endif //ENTITYMESSAGE_H
