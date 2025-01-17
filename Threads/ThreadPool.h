#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <vector>
#include "../Misc/PublicEnums.h"
#include "../Misc/PublicStructs.h"

#include "ThreadQueue.h"


namespace Engine {

    class ThreadPool {
    public:
        explicit ThreadPool(std::size_t n_threads);
        ~ThreadPool();
        bool push(Task const& task);
        private:
        ThreadQueue<Task> queue;
        std::vector<std::jthread> threads;

    private:

        static auto make_thread_handler(ThreadQueue<Task>& queue) {
            return std::jthread{
                [&queue] {
                    while (true) {
                        auto const elem = queue.pop();
                        switch (elem.type) {
                            case TaskType::Execute:
                                elem.task(elem.arguments);
                            break;
                            case TaskType::Stop:
                                return;
                        }
                    }
                }};
        }
    };
}



#endif //THREADPOOL_H
