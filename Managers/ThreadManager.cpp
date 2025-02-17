#include "ThreadManager.h"

namespace Engine {
    ThreadManager* ThreadManager::instance = nullptr;

    ThreadManager::ThreadManager(): threadPool(2), running(false) {
    }

    void ThreadManager::Allocate() {
        assert(instance == nullptr);
        if (instance != nullptr)
            return;
        instance = new ThreadManager();
    }

    ThreadManager * ThreadManager::Instance() {
        return instance;
    }

    ThreadPool * ThreadManager::GetThreadPool() {
        return &threadPool;
    }

    void ThreadManager::QueueTask(const std::function<void(MeshManager&, const string&)>& func,
            const Param& param,
            MeshManager& manager,
            TaskType type) {

        Task task {type};
        task.task = [this, &manager, func](Param param) {
            std::lock_guard<std::mutex> lock(mutex);
            func(manager, std::get<string>(param));

        };
        task.arguments = param;
        threadPool.push(task);
    }
}


