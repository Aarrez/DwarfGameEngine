#ifndef MEMORY_H
#define MEMORY_H

#ifdef WIN32
#include <windows.h>
#include <iostream>

namespace Engine {
  class Memory {
    public:
    static void PrintMemory(){
      MEMORYSTATUSEX statex;
      statex.dwLength = sizeof(statex);
      GlobalMemoryStatusEx(&statex);
      std::cout << "There is " << statex.ullTotalPhys / (1024 * 1024)
      <<  " MB of physical memory available." << std::endl;
      std::cout << "There is " << statex.ullAvailVirtual / (1024 * 1024)
      << " MB of virtual memory available" << std::endl;
    };
    private:
      Memory() = default;
  };
}
#endif

#endif //MEMORY_H
