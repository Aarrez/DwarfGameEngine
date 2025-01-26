#include "ThreadManager.h"

#include "EntityManager.h"
#include "EntityManager.h"


namespace Engine {
    ThreadManager* ThreadManager::instance = nullptr;

    ThreadManager::ThreadManager():
    threadPool(2), running(false){
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

    void ThreadManager::QueueTask(std::function<Mesh(MeshManager &, const string &)> func,
        const string &param,
        MeshManager &manager,
        TaskType type) {
        Task task {type};
        func(manager, param);
        /*task.task = {[this, func, param, manager](std::vector<Param> params) {
            {
                std::lock_guard lock(mutex);

            }
        }};*/
        threadPool.push(task);
    }
}


