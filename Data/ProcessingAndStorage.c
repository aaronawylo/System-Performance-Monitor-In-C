//
// Created by Maddelin on 11/14/2023.
//

#include <windows.h>
#include <pdh.h>
#include "ProcessingAndStorage.h"
#include "../CPU/CPUUtilities.h"
#include "../Network/NetworkUtilities.h"
#include "../Disk_IO/DiskIO_monitoring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure to represent performance data
struct PerformanceData {
    char timestamp[20];
    double cpu;
    double memory;
    double diskIO;
    double totalNetworkTraffic;
};

//// Function to process data
//void processData(struct PerformanceData *data, int dataSize) {
//    double totalCpu = 0, totalMemory = 0, totalDiskIO = 0, totalNetworkUsage = 0;
//    int count = 0;
//
//    for (int i = 0; i < dataSize; ++i) {
//        totalCpu += data[i].cpu;
//        totalMemory += data[i].memory;
//        totalDiskIO += data[i].diskIO;
//        totalNetworkUsage += data[i].totalNetworkTraffic;
//        count++;
//    }
//
//    // Check if count is not zero to avoid division by zero
//    if (count != 0) {
//        double averageCpu = totalCpu / count;
//        double averageMemory = totalMemory / count;
//        double averageDiskIO = totalDiskIO / count;
//        double averageNetworkUsage = totalNetworkUsage / count;
//
//        printf("\nAverage CPU: %.2f%%\n", averageCpu);
//        printf("Average Memory: %.2f%%\n", averageMemory);
//        printf("Average Disk IO: %.2f\n", averageDiskIO);
//        printf("Average Network Usage: %.2f\n", averageNetworkUsage);
//    } else {
//        printf("No data points to calculate averages.\n");
//    }
//}

// Function to save data to a file
void saveData(struct PerformanceData *data, int dataSize, const char *filename) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dataSize; ++i) {
        fprintf(file, "Timestamp: %s\n"
                      "CPU Usage: %.2lf\n"
                      "Memory Usage: %.2lf\n"
                      "Disk Usage: %.2lf\n"
                      "Total Network Traffic: %.2lf\n\n",
                data[i].timestamp, data[i].cpu, data[i].memory,
                data[i].diskIO, data[i].totalNetworkTraffic);
    }

    fclose(file);
}

// Function to load data from a file
void loadData(struct PerformanceData **data, int *dataSize, const char *filename) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        // If the file doesn't exist, create a new file
        file = fopen(filename, "w");
        if (file == NULL) {
            perror("Error creating file");
            exit(EXIT_FAILURE);
        }
        fclose(file);

        // Reattempt to open the newly created file
        file = fopen(filename, "r");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
    }

    // Allocate memory for the array
    *dataSize = 0;  // Initialize size to 0
    *data = NULL;   // Initialize to NULL

    // Read data from the file
    struct PerformanceData temp;
    while (fscanf(file,
                  "Timestamp: %s\n"
                  "CPU Usage: %lf\n"
                  "Memory Usage: %lf\n"
                  "Disk Usage: %lf\n"
                  "Total Network Traffic: %lf\n\n",
                  temp.timestamp, &temp.cpu, &temp.memory,
                  &temp.diskIO, &temp.totalNetworkTraffic) == 5) {
        (*dataSize)++;
        *data = realloc(*data, (*dataSize) * sizeof(struct PerformanceData));
        if (*data == NULL) {
            perror("Error reallocating memory");
            exit(EXIT_FAILURE);
        }
        // Copy the temp data to the allocated array
        (*data)[*dataSize - 1] = temp;
    }

    fclose(file);
}

double calculateDiskUsagePercentage() {
    char diskName[] = "C";

    ULARGE_INTEGER total, free, totalfree;

    if (!GetDiskFreeSpaceEx(diskName, &total, &totalfree, &free)) {
        DWORD error = GetLastError();
        if (error == ERROR_PATH_NOT_FOUND) {
            printf("Error: Disk not found or path not available.\n");
        } else {
            printf("Error getting disk space. Error code: %lu\n", error);
        }
        // Handle the error as needed
        return -1.0;
    }

    double diskTotal = getDiskTotal(total);
    double diskFree = getDiskFreeSpace(free);

    if (diskTotal > 0) {
        double usagePercentage = ((diskTotal - diskFree) / diskTotal) * 100;
        return usagePercentage;
    } else {
        return 0.0;
    }
}

int main() {
    // Initialize PDH Query
    setupPdhQuery();
    SetupSentQuery();
    SetupReceivedQuery();

    // Load existing data from a file
    struct PerformanceData *loadedData;
    int loadedDataSize;
    loadData(&loadedData, &loadedDataSize, "performance_data.txt");

    // Infinite loop to continuously retrieve and process real-time data
    while (1) {
        // Get current timestamp
        SYSTEMTIME localTime;
        GetLocalTime(&localTime);
        char timestamp[20];
        sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d",
                localTime.wYear, localTime.wMonth, localTime.wDay,
                localTime.wHour, localTime.wMinute, localTime.wSecond);

        // Get real-time performance stats
        double currentCpuUsage = getCurrentCpuUsage();
        double currentMemoryUsage = 0;
        double currentDiskUsage = calculateDiskUsagePercentage();
        double currentNetworkSent = GetNetworkSent();
        double currentNetworkReceived = GetNetworkReceived();

        // Process and display data
        struct PerformanceData currentData = {
                .cpu = currentCpuUsage,
                .memory = currentMemoryUsage,
                .diskIO = currentDiskUsage,
                .totalNetworkTraffic = currentNetworkSent + currentNetworkReceived
        };

        strcpy(currentData.timestamp, timestamp);

        // Append real-time data to loaded data
        loadedDataSize++;
        loadedData = realloc(loadedData, loadedDataSize * sizeof(struct PerformanceData));
        if (loadedData == NULL) {
            perror("Error reallocating memory");
            exit(EXIT_FAILURE);
        }
        loadedData[loadedDataSize - 1] = currentData;

        // Save data to a file
        saveData(loadedData, loadedDataSize, "performance_data.txt");

        // Sleep for a short duration (e.g., 1 second) before the next iteration
        Sleep(1000);
    }
    return 0;
}
