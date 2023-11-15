//
// Created by Maddelin on 11/14/2023.
//

#ifndef DATA_PROCESSING_AND_STORAGE_H
#define DATA_PROCESSING_AND_STORAGE_H

struct PerformanceData;
void processData(struct PerformanceData *data, int dataSize);
void saveData(struct PerformanceData *data, int dataSize, const char *filename);
void loadData(struct PerformanceData **data, int *dataSize, const char *filename);

#endif // DATA_PROCESSING_AND_STORAGE_H
