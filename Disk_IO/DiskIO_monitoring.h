//
// Created by Abdulqadir Abuharrus on 2023-11-14.
//

#ifndef DISKIO_MONITORING_H
#define DISKIO_MONITORING_H


double getDiskTotal(ULARGE_INTEGER total);
double getDiskFreeSpace(ULARGE_INTEGER free);
double getDiskTotalFree();
double getDiskRead(const char *diskName, DISK_PERFORMANCE diskPerformance);
double getDiskWrite(const char *diskName, DISK_PERFORMANCE diskPerformance);
double calculateDiskUsagePercentage();

#endif //DISKIO_MONITORING_H