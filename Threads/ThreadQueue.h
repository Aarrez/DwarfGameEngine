#ifndef THREADQUEUE_H
#define THREADQUEUE_H
#include <condition_variable>
#include <queue>

namespace Engine {
    template <typename T>
    class ThreadQueue {
    public:
        void push(T const& val) {
            std::lock_guard<std::mutex> queue_lock{queueMutex};
            queue.push(val);
            queueConVal.notify_one();
        }
        T pop() {
            std::unique_lock<std::mutex> queue_lock{queueMutex};
            queueConVal.wait(queue_lock, [&]{return !queue.empty();});
            T ret = queue.front();
            queue.pop();
            return ret;
        }

        private:
        std::queue<T> queue;
        std::condition_variable queueConVal;
        std::mutex queueMutex;
    };


}



#endif //THREADQUEUE_H
