#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <iostream>
#include <thread>

#include <vector>
#include "../Misc/PublicEnums.h"
#include "../Misc/PublicStructs.h"

#include "ThreadQueue.h"


namespace Engine {

    class ThreadPool {
    public:
        ThreadPool(std::size_t n_threads);
        ~ThreadPool();
        bool push(Task task);
        private:
        ThreadQueue<Task> queue {};
        std::vector<std::thread> threads;
        size_t m_n_threads;
        void JoinThreads();

    private:

        std::thread make_thread_handler(ThreadQueue<Task>& queue) {
            return std::thread{
                [&queue, this] {
                    while (true) {
                        Task const elem = queue.pop();
                        switch (elem.type) {
                            case TaskType::Execute:
                                elem.task(elem.arguments);
                            break;
                            case TaskType::Stop:
                                if (m_n_threads == 0) {
                                    JoinThreads();
                                }
                                m_n_threads--;
                                return;
                        }
                    }
                }};
        }
    };

    inline void ThreadPool::JoinThreads() {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
}



#endif //THREADPOOL_H
