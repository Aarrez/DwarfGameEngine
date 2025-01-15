//
// Created by Aaron.Marklund on 2025-01-15.
//

#include "Message.h"

#include <utility>


namespace Engine {
    Message::Message(const MessageType type, std::string  message):
    mType(type), mMessage(std::move(message)) {
    }

    Message::~Message() = default;

}