
//
// Created by Aaron on 11/14/2023.
//

//#include <windows.h>
//#include <psapi.h>
//#include "Memory.h"
//
//int getMemoryUsage() {
//    PROCESS_MEMORY_COUNTERS pmc;
//
//    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
//        // Print memory usage information
//        return (int)(pmc.WorkingSetSize / 1024);
//    } else {
//        return 1;
//    }
//}

#include <windows.h>
#include <stdio.h>
#include <psapi.h>

// Function to get current RAM usage in kilobytes
int getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        // Return current RAM usage in kilobytes
        return (int)(pmc.WorkingSetSize / 1024);
    } else {
        // Error occurred
        fprintf(stderr, "Failed to retrieve memory information.\n");
        return -1;
    }
}

// Function to get total available RAM in kilobytes
int getTotalAvailableMemory() {
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (GlobalMemoryStatusEx(&memoryStatus)) {
        // Return total available RAM in kilobytes
        return (int)(memoryStatus.ullAvailPhys / 1024);
    } else {
        // Error occurred
        fprintf(stderr, "Failed to retrieve total available memory.\n");
        return -1;
    }
}

// Function to calculate and return the percentage of RAM used
double getMemoryUsagePercentage() {
    int currentMemoryUsage = getCurrentMemoryUsage();
    int totalAvailableMemory = getTotalAvailableMemory();

    if (currentMemoryUsage >= 0 && totalAvailableMemory >= 0) {
        // Calculate the percentage of used memory
        double percentage = ((double)currentMemoryUsage / totalAvailableMemory) * 100.0;
        return percentage;
    } else {
        // Error occurred
        fprintf(stderr, "Failed to calculate memory usage percentage.\n");
        return -1.0;
    }
}

int main() {
    // Call the function to get the percentage of RAM used
    double usagePercentage = getMemoryUsagePercentage();

    if (usagePercentage >= 0) {
        printf("Memory Usage Percentage: %.2f%%\n", usagePercentage);
    }

    return 0;
}
