#include "Memory.h"


namespace Dwarf {
    namespace Memory {
        DWORD Memory::GetPhysicalMemoryAvailable() {
            MEMORYSTATUSEX stat;
            stat.dwLength = sizeof(stat);
            GlobalMemoryStatusEx(&stat);
            return stat.ullAvailPhys/(1024*1024);
        }

        DWORD Memory::GetVirtualMemoryAvailable() {
            MEMORYSTATUSEX stat;
            stat.dwLength = sizeof(stat);
            GlobalMemoryStatusEx(&stat);
            return stat.ullAvailVirtual/(1024*1024);
        }

    }
}
