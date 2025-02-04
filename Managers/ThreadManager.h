#ifndef DWARFTHREADING_H
#define DWARFTHREADING_H
#include "MeshManager.h"
#include "../Threads/ThreadPool.h"


namespace Engine {
    class ThreadManager {
    private:
        ThreadManager();
        static ThreadManager* instance;
        ThreadPool threadPool;
        std::mutex mutex;
        bool running;

    public:
        static void Allocate();
        static ThreadManager* Instance();
        ThreadPool* GetThreadPool();
        void QueueTask(std::function<Mesh(MeshManager&, const string&)>& func,
            const Param& param,
            MeshManager& manager,
            TaskType type = TaskType::Execute) {

            Task task {type};
            task.task = [this, &manager, func](Param param) {
                std::lock_guard<std::mutex> lock(mutex);
                func(manager, std::get<string>(param));
            };
            task.arguments = param;
            threadPool.push(task);
        };
    };
}

#endif //DWARFTHREADING_H
