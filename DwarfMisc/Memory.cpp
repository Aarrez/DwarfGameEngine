#include "Memory.h"


namespace Dwarf {
    DWORD Memory::GetPhysicalMemoryAvailable() {
        MEMORYSTATUSEX stat;
        stat.dwLength = sizeof(stat);
        GlobalMemoryStatusEx(&stat);
        return stat.ullTotalPhys/(1024*1024);
    }

    DWORD Memory::GetVirtualMemoryAvailable() {
        MEMORYSTATUSEX stat;
        stat.dwLength = sizeof(stat);
        GlobalMemoryStatusEx(&stat);
        return stat.ullTotalVirtual/(1024*1024);
    }

}
