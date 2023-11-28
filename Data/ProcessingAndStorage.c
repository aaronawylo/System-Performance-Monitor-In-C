//
// Created by Maddelin on 11/14/2023.
//

#include <windows.h>
#include "ProcessingAndStorage.h"

#include <stdio.h>
#include <stdlib.h>

// Function to save data to a file
void saveData(struct PerformanceData *data, const char *filename) {
   FILE *file = fopen(filename, "a");

   if (file == NULL) {
       perror("Error opening file");
       exit(EXIT_FAILURE);
   }

   fprintf(file, "Timestamp: %s\n"
                 "CPU Usage: %.2lf%%\n"
                 "Memory Usage: %.2lf%%\n"
                 "Disk Usage: %.2lf%%\n"
                 "Network Sent: %.2lf\n"
                 "Network Received: %.2lf\n\n",
           data->timestamp, data->cpu, data->memory,
           data->diskUsage, data->networkSent, data->networkReceived);


   fclose(file);
}
