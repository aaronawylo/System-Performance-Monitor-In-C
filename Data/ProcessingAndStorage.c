//
// Created by Maddelin on 11/14/2023.
//

#include <windows.h>
#include <pdh.h>
#include "ProcessingAndStorage.h"
#include "../CPU/CPUUtilities.h"
#include "../Network/NetworkUtilities.h"
#include "../Disk_IO/DiskIO_monitoring.h"
#include "../Memory/Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 50

// Define a structure to represent performance data
struct PerformanceData {
    char timestamp[20];
    double cpu;
    int memory;
    double diskIO;
    double totalNetworkTraffic;
};

// Function to save data to a file
void saveData(struct PerformanceData *data, const char *filename) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "Timestamp: %s\n"
                  "CPU Usage: %.2lf\n"
                  "Memory Usage: %d\n"
                  "Disk Usage: %.2lf\n"
                  "Total Network Traffic: %.2lf\n\n",
            data->timestamp, data->cpu, data->memory,
            data->diskIO, data->totalNetworkTraffic);

    fclose(file);
}

double calculateDiskUsagePercentage() {
    char diskName[] = "C:";

    ULARGE_INTEGER free, total, totalfree;

    if (!GetDiskFreeSpaceEx(diskName, &total, &totalfree, &free)) {
//    DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;
//    if (!GetDiskFreeSpace(diskName, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters)) {
        DWORD error = GetLastError();
        fprintf(stderr, "Error getting disk space. Error code: %lu\n", error);
        return -1.0;
    }

    double diskTotal = getDiskTotal(total);
    double diskTotalFree = getDiskTotalFree(totalfree);

    if (diskTotal > 0) {
        return ((diskTotal - diskTotalFree) / diskTotal) * 100;
    } else {
        return 0.0;
    }
}

double calculateTotalNetworkTraffic() {
    return GetNetworkSent() + GetNetworkReceived();
}

//int main() {
//    // Initialize PDH Query
//    setupPdhQuery();
//    SetupSentQuery();
//    SetupReceivedQuery();
//
//    struct PerformanceData *currentData = malloc(sizeof(struct PerformanceData));
//    if (currentData == NULL) {
//        perror("Error allocating memory");
//        exit(EXIT_FAILURE);
//    }
//    int currentDataSize = 0;
//
//    // Infinite loop to continuously retrieve and process real-time data
//    while (1) {
//        // Get current time
//        SYSTEMTIME localTime;
//        GetLocalTime(&localTime);
//        char timestamp[20];
//        sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d",
//                localTime.wYear, localTime.wMonth, localTime.wDay,
//                localTime.wHour, localTime.wMinute, localTime.wSecond);
//
//        strcpy( currentData->timestamp, timestamp);
//
//        // Retrieve data
//        currentData->cpu = getCurrentCpuUsage();
//        currentData->memory = getMemoryUsage();
//        currentData->diskIO = calculateDiskUsagePercentage();
//        currentData->totalNetworkTraffic = calculateTotalNetworkTraffic();
//
//        currentDataSize++;
//
//        saveData(currentData, "performance_data.txt");
//
//        // Sleep for a short duration (e.g., 1 second) before the next iteration
//        Sleep(1000);
//    }
//    // Free allocated memory before exiting
//    free(currentData);
//    return 0;
//}
