//
// Created by Maddelin on 11/14/2023.
//

#include <windows.h>
#include "ProcessingAndStorage.h"
#include "../CPU/CPUUtilities.h"
#include "../Network/NetworkUtilities.h"
#include "../Disk_IO/DiskIO_monitoring.h"
#include "../Memory/Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to save data to a file
void saveData(struct PerformanceData *data, const char *filename) {
   FILE *file = fopen(filename, "a");

   if (file == NULL) {
       perror("Error opening file");
       exit(EXIT_FAILURE);
   }

   fprintf(file, "Timestamp: %s\n"
                 "CPU Usage: %.2lf%\n"
                 "Memory Usage: %.2lf%\n"
                 "Disk Usage: %.2lf%\n"
                 "Network Sent: %.2lf\n"
                 "Network Received: %.2lf\n\n",
           data->timestamp, data->cpu, data->memory,
           data->diskUsage, data->networkSent, data->networkReceived);


   fclose(file);
}


int mainData() {
   // Initialize PDH Query
   setupPdhQuery();
   SetupSentQuery();
   SetupReceivedQuery();

   struct PerformanceData *currentData = malloc(sizeof(struct PerformanceData));
   if (currentData == NULL) {
       perror("Error allocating memory");
       exit(EXIT_FAILURE);
   }
   int currentDataSize = 0;

   // Infinite loop to continuously retrieve and process real-time data
   while (1) {
       // Get current time
       SYSTEMTIME localTime;
       GetLocalTime(&localTime);
       char timestamp[20];
       sprintf(timestamp, "%04d-%02d-%02d %02d:%02d:%02d",
               localTime.wYear, localTime.wMonth, localTime.wDay,
               localTime.wHour, localTime.wMinute, localTime.wSecond);

       strcpy( currentData->timestamp, timestamp);

       // Retrieve data
       currentData->cpu = getCurrentCpuUsage();
       currentData->memory = getMemoryUsagePercentage();
       currentData->diskUsage = calculateDiskUsagePercentage();
       currentData->networkSent = GetNetworkSent();
       currentData->networkReceived = GetNetworkReceived();

       currentDataSize++;

       saveData(currentData, "performance_data.txt");

       // Sleep for a short duration (e.g., 1 second) before the next iteration
       Sleep(1000);
   }
   // Free allocated memory before exiting
   free(currentData);
   return 0;
}
