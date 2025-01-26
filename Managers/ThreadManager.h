#ifndef DWARFTHREADING_H
#define DWARFTHREADING_H

#include <thread>
#include <cassert>

#include "MeshManager.h"
#include "../Threads/ThreadPool.h"


namespace Engine {
    typedef void (* funcCall)(const string&);
    class ThreadManager {
    private:
        ThreadManager();
        static ThreadManager* instance;
        ThreadPool threadPool;
        std::mutex mutex;
        bool running;
        thread* t1;

    public:
        static void Allocate();
        static ThreadManager* Instance();
        ThreadPool* GetThreadPool();
        /*void QueueTask(std::function<Param(const Param&)> func, const Param& param, TaskType type = TaskType::Execute);

        void QueueTask(Mesh(MeshManager::* func)(const string &filePath)&, Param param, TaskType type = TaskType::Execute);*/
        void QueueTask(std::function<Mesh(MeshManager&, const string&)> func,
            const string& param,
            MeshManager& meshManager,
            TaskType type = TaskType::Execute);
    };
}




#endif //DWARFTHREADING_H
