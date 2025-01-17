#ifndef THREADQUEUE_H
#define THREADQUEUE_H
#include <condition_variable>
#include <queue>

namespace Engine {
    template <typename T>
    class ThreadQueue {
    public:
        void push(T const& val);
        T pop();
        private:
        std::queue<T> queue;
        std::condition_variable queueConVal;
        std::mutex queueMutex;
    };


}



#endif //THREADQUEUE_H
