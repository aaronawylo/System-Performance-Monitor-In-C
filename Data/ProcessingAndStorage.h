//
// Created by Maddelin on 11/14/2023.
//

#ifndef DATA_PROCESSING_AND_STORAGE_H
#define DATA_PROCESSING_AND_STORAGE_H

struct PerformanceData {
    char timestamp[20];
    double cpu;
    double memory;
    double diskUsage;
    double networkSent;
    double networkReceived;
};
void saveData(struct PerformanceData *data, const char *filename);

#endif // DATA_PROCESSING_AND_STORAGE_H
