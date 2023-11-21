// process_utils.c
#include "process.h"
#include <psapi.h>
#include <stdio.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);

    if (lpdwProcessId == (DWORD) lParam) {
        char windowTitle[256];
        if (GetWindowText(hwnd, windowTitle, sizeof(windowTitle)) > 0) {
            if (IsWindowVisible(hwnd)) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOL IsUserApplication(DWORD processID) {
    return !EnumWindows(EnumWindowsProc, (LPARAM) processID);
}

void PrintProcesses() {
    DWORD processes[1024], needed, cProcesses;
    unsigned int i;

    if (!EnumProcesses(processes, sizeof(processes), &needed)) {
        printf("EnumProcesses failed\n");
        return;
    }

    cProcesses = needed / sizeof(DWORD);

//    printf("Number of processes: %u\n", cProcesses);
    printf("%-15s%-30s%-10s\n", "Process ID", "Process Name", "Running");
    printf("--------------------------------------------------------------------------------\n");

    for (i = 0; i < cProcesses; i++) {
        if (processes[i] != 0) {
            char processName[MAX_PATH] = "<unknown>";
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);

            if (hProcess != NULL) {
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    GetModuleBaseName(hProcess, hMod, processName, sizeof(processName) / sizeof(char));
                }

                if (strcmp(processName, "<unknown>") != 0 && IsUserApplication(processes[i])) {
                    printf("%-15lu%-30s%-10s\n", processes[i], processName, "Running");
                }

                CloseHandle(hProcess);
            }
        }
    }
}

//int main() {
//    PrintProcesses();
//    return 0;
//}
