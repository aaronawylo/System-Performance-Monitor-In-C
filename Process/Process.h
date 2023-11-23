//
// Created by Mika on 2023-11-14.
//

// process_utils.h
#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

#include <windows.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
BOOL IsUserApplication(DWORD processID);
void PrintProcesses();


#endif // PROCESS_UTILS_H
