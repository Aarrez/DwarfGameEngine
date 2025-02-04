#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <queue>

#include "Message.h"

namespace Engine {
  template <typename T>
  class MessageQueue {
  public:
    MessageQueue(){}
    ~MessageQueue(){}

    void push(const T& msg) {
      messages.push(msg);
    }

    T pop() {
      T front = messages.front();
      messages.pop();
      return front;
    }
  private:
    std::queue<T> messages;
  };

}


#endif //MESSAGEQUEUE_H
