
//
// Created by Aaron on 11/14/2023.
//

#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include "Memory.h"

int getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;

    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        // Print memory usage information
        return (int)(pmc.WorkingSetSize / 1024);
    } else {
        return 1;
    }
}

// Function to get the total memory usage of all processes in kilobytes
int getCurrentMemoryUsage() {
    DWORD processes[1024];
    DWORD cbNeeded;
    if (EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
        // Calculate the number of processes
        DWORD numProcesses = cbNeeded / sizeof(DWORD);

        // Variables to store total memory usage
        SIZE_T totalMemoryUsage = 0;

        // Iterate through each process
        for (DWORD i = 0; i < numProcesses; i++) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
            if (hProcess != NULL) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    // Add the working set size to the total memory usage
                    totalMemoryUsage += pmc.WorkingSetSize;
                }
                CloseHandle(hProcess);
            }
        }

        // Convert total memory usage to kilobytes
        return (int)(totalMemoryUsage / 1024);
    } else {
        // Error occurred
        fprintf(stderr, "Failed to enumerate processes.\n");
        return -1;
    }
}

// Function to get total available RAM in kilobytes
int getTotalAvailableMemory() {
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    if (GlobalMemoryStatusEx(&memoryStatus)) {
        // Return total available RAM in kilobytes
        return (int)(memoryStatus.ullTotalPhys / 1024);
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
        double percentage = ((double)currentMemoryUsage / (double)totalAvailableMemory) * 100.0;
        return percentage;
    } else {
        // Error occurred
        fprintf(stderr, "Failed to calculate memory usage percentage.\n");
        return -1.0;
    }
}
