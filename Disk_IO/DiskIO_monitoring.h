//
// Created by Abdulqadir Abuharrus on 2023-11-14.
//

#ifndef DISKIO_MONITORING_H
#define DISKIO_MONITORING_H


double getDiskTotal(ULARGE_INTEGER total);
double getDiskFreeSpace(ULARGE_INTEGER free);
double getDiskTotalFree();
double getDiskRead();
double getDiskWrite();

#endif //DISKIO_MONITORING_H