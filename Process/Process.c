#include "Process.h"
#include <windows.h>
#include <psapi.h>
#include <stdio.h>

BOOL CheckVisibleWindow(HWND hwnd, LPARAM lParam) {
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
    return !EnumWindows(CheckVisibleWindow, (LPARAM) processID);
}

void PrintProcesses(ProcessInfo *processList, int maxProcesses, int *numProcesses) {
    DWORD processes[1024], needed, cProcesses;
    unsigned int i;

    if (!EnumProcesses(processes, sizeof(processes), &needed)) {
        printf("EnumProcesses failed\n");
        return;
    }

    cProcesses = needed / sizeof(DWORD);
    *numProcesses = 0;

    for (i = 0; i < cProcesses && *numProcesses < maxProcesses; i++) {
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
                    processList[*numProcesses].processId = processes[i];
                    strcpy(processList[*numProcesses].processName, processName);
                    strcpy(processList[*numProcesses].status, "Running");
                    (*numProcesses)++;
                }

                CloseHandle(hProcess);
            }
        }
    }
}
