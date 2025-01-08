#ifndef MEMORY_H
#define MEMORY_H
namespace Dwarf {
    namespace Memory {
#include <Windows.h>
        DWORD GetPhysicalMemoryAvailable();
        DWORD GetVirtualMemoryAvailable();
    }

}
#endif //MEMORY_H
