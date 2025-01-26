#include "ThreadPool.h"



namespace Engine {
    ThreadPool::ThreadPool(std::size_t n_threads) {
        for (std::size_t i = 0; i < n_threads; i++) {
            threads.push_back(make_thread_handler(queue));
        }
    }

    ThreadPool::~ThreadPool() {
        Task stop_task {TaskType::Stop, {}, {}};
        for (std::size_t i = 0; i< threads.size(); ++i) {
            push(stop_task);
        }
    }

    bool ThreadPool::push(Task task) {
        queue.push(task);
        return true;
    }


}


