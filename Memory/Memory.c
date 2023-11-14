
//
// Created by Aaron on 11/14/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <psapi.h>

void print_memory_usage() {
    PROCESS_MEMORY_COUNTERS pmc;

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        // Print memory usage information
        printf("Memory usage: %u KB\n", (unsigned int)(pmc.WorkingSetSize / 1024));
    } else {
        perror("Error getting process memory info");
    }
}

int memoryMain() {
    int i = 0;
    while (i < 5) {
        // Print memory usage every 5 seconds
        print_memory_usage();
        Sleep(5000);  // Sleep for 5 seconds (note: Sleep uses milliseconds)
        i += 1;
    }

    return 0;
}
