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
        void QueueTask(const std::function<void(MeshManager&, const string&)>& func,
            const Param& param,
            MeshManager& manager,
            TaskType type = TaskType::Execute);

    };
}

#endif //DWARFTHREADING_H
