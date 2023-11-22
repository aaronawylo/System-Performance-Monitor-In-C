//
// Created by Maddelin on 11/14/2023.
//

#ifndef DATA_PROCESSING_AND_STORAGE_H
#define DATA_PROCESSING_AND_STORAGE_H

struct PerformanceData;
void saveData(struct PerformanceData *data, const char *filename);
double calculateDiskUsagePercentage();
double calculateTotalNetworkTraffic();

#endif // DATA_PROCESSING_AND_STORAGE_H
