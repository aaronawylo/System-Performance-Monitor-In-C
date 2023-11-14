#include <stdio.h>
#include <windows.h>

void getDiskSpace(const char* diskName, ULARGE_INTEGER total, ULARGE_INTEGER free, ULARGE_INTEGER totalfree) {
    printf("Disk %s - Total space: %.2f MB\n", diskName, (double)total.QuadPart / (1024 * 1024));
    printf("Disk %s - Free space: %.2f MB\n", diskName, (double)free.QuadPart / (1024 * 1024));
    printf("Disk %s - Available space: %.2f MB\n", diskName, (double)totalfree.QuadPart / (1024 * 1024));
}

void getDiskIO(const char* diskName, DISK_PERFORMANCE diskPerformance) {
    printf("Disk %s - Read Bytes: %.2f MB\n", diskName, (double)diskPerformance.BytesRead.QuadPart / (1024 * 1024));
    printf("Disk %s - Write Bytes: %.2f MB\n", diskName, (double)diskPerformance.BytesWritten.QuadPart / (1024 * 1024));
}

int main() {
    char diskName[] = "C";
    ULARGE_INTEGER total, free, totalfree;
    GetDiskFreeSpaceEx(diskName, &total, &totalfree, &free);
    getDiskSpace(diskName, total, free, totalfree);

    // Read and write statistics for specified disk
    HANDLE hDevice = CreateFile("\\\\.\\C:", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Error opening disk\n");
        return 1;
    }

    DISK_PERFORMANCE diskPerformance;
    DWORD bytesReturned;
    if (!DeviceIoControl(hDevice, IOCTL_DISK_PERFORMANCE, NULL, 0, &diskPerformance, sizeof(diskPerformance), &bytesReturned, NULL)) {
        printf("Error getting disk performance\n");
        CloseHandle(hDevice);
        return 1;
    }

    getDiskIO(diskName, diskPerformance);

    CloseHandle(hDevice);
    return 0;
}
