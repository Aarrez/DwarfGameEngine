#ifndef MEMORY_H
#define MEMORY_H
#include <Windows.h>
#include <iostream>
namespace Dwarf {
    namespace Memory {
        static DWORD GetPhysicalMemoryAvailable();
        static DWORD GetVirtualMemoryAvailable();
    }

}
#endif //MEMORY_H
