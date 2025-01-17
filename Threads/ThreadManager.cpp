#include "ThreadManager.h"



namespace Engine {
    ThreadManager* ThreadManager::instance = nullptr;

    ThreadManager::ThreadManager():
    threadPool(4), running(false), jobNr(0) {
        t1 = new thread(Update);
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

    void ThreadManager::Update() {
        //TODO figure out how to give worker threads a task
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void ThreadManager::Deallocate() {
        if (t1->joinable()) {
            t1->join();
            delete t1;
            t1 = nullptr;
        }
    }
}


