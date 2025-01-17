#ifndef DWARFTHREADING_H
#define DWARFTHREADING_H

#include <thread>
#include <cassert>
#include "ThreadPool.h"


namespace Engine {
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
        void Deallocate();
        void Update();
        void QueueTask();
        int jobNr;


    };
}




#endif //DWARFTHREADING_H
