#include "ThreadQueue.h"


namespace Engine {
    template <typename T>
    void ThreadQueue<T>::push(T const &val) {
        std::lock_guard<std::mutex> queue_lock{queueMutex};
        queue.push(val);
        queueConVal.notify_one();
    }

    template<typename T>
    T ThreadQueue<T>::pop() {
        std::unique_lock<std::mutex> queue_lock{queueMutex};
        queueConVal.wait(queue_lock, [&]{return !queue.empty();});
        T ret = queue.front();
        queue.pop();
        return ret;
    }
}
