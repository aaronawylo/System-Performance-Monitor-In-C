
//
// Created by Aaron on 11/14/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <psapi.h>

int getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        // Print memory usage information
        return (unsigned int)(pmc.WorkingSetSize / 1024);
    } else {
        return 1;
    }
}
