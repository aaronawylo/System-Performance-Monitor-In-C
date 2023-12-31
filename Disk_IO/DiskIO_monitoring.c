#include <stdio.h>
#include <windows.h>

DISK_PERFORMANCE diskPerformance;
char diskName[] = "C:\\";

void setupReadWriteQuery() {
    // Read and write statistics for specified disk
    HANDLE hDevice = CreateFile("\\\\.\\C:", 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Error opening disk\n");
    }

    DWORD bytesReturned;
    if (!DeviceIoControl(hDevice, IOCTL_DISK_PERFORMANCE, NULL, 0,
                         &diskPerformance, sizeof(diskPerformance),
                         &bytesReturned, NULL)) {
        printf("Error getting disk performance\n");
        CloseHandle(hDevice);
    }
}

double getDiskTotal(ULARGE_INTEGER total) {
    double diskTotal = (double) total.QuadPart / (1024 * 1024);
    return diskTotal;
}

double getDiskFreeSpace(ULARGE_INTEGER free) {
    double diskFree = (double) free.QuadPart / (1024 * 1024);
    return diskFree;
}

double getDiskTotalFree(ULARGE_INTEGER totalfree) {
    double diskTotalFree = (double) totalfree.QuadPart / (1024 * 1024);
    return diskTotalFree;
}

double getDiskRead() {
    double readRate = (double) diskPerformance.BytesRead.QuadPart / (1024 * 1024);
    return readRate;
}

double getDiskWrite() {
    double writeRate = (double) diskPerformance.BytesWritten.QuadPart / (1024 * 1024);
    return writeRate;
}

double calculateDiskUsagePercentage() {
    ULARGE_INTEGER total, free, totalfree;

    if (!GetDiskFreeSpaceEx(diskName, &free, &total, &totalfree)) {
        printf("Error getting disk space information\n");
        return 1;
    }
    GetDiskFreeSpaceEx(diskName, &free, &total,
                       &totalfree);

    double diskTotal = getDiskTotal(total);
    double diskTotalFree = getDiskTotalFree(totalfree);

    if (diskTotal > 0) {
        return ((diskTotal - diskTotalFree) / diskTotal) * 100;
    } else {
        return 1;
    }
}

int mainDisk() {
    ULARGE_INTEGER total, free, totalfree;

    if (!GetDiskFreeSpaceEx(diskName, &free, &total, &totalfree)) {
        printf("Error getting disk space information\n");
        return 1;
    }
    GetDiskFreeSpaceEx(diskName, &free, &total,
                       &totalfree);


    printf("Disk %s - Total Bytes: %.2f MB\n", diskName, getDiskTotal(total));
    printf("Disk %s - Free Bytes: %.2f MB\n", diskName, getDiskFreeSpace(free));
    printf("Disk %s - Available Bytes: %.2f MB\n", diskName, getDiskTotalFree(totalfree));

    // Read and write statistics for specified disk
    HANDLE hDevice = CreateFile("\\\\.\\C:", 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Error opening disk\n");
        return 1;
    }

    DWORD bytesReturned;
    if (!DeviceIoControl(hDevice, IOCTL_DISK_PERFORMANCE, NULL, 0,
                         &diskPerformance, sizeof(diskPerformance),
                         &bytesReturned, NULL)) {
        printf("Error getting disk performance\n");
        CloseHandle(hDevice);
        return 1;
    }
    printf("Disk %s - Read Bytes: %.2f MB\n", diskName, getDiskRead(diskName, diskPerformance));
    printf("Disk %s - Write Bytes: %.2f MB\n", diskName, getDiskWrite(diskName, diskPerformance));


    CloseHandle(hDevice);
    return 0;
}
