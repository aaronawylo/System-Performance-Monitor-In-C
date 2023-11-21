//
// Created by Mika on 2023-11-20.
//

#include "CurrentUser.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

char* getCurrentUserName() {
    static char userName[256];

#ifdef _WIN32
    DWORD userNameSize = sizeof(userName);
    if (!GetUserName(userName, &userNameSize)) {
        strcpy(userName, "Unknown User");
    }
#else
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if (pw) {
        strcpy(userName, pw->pw_name);
    } else {
        strcpy(userName, "Unknown User");
    }
#endif

    return userName;
}

void getCurrentDateTime(char* buffer, int bufferSize) {
    time_t now = time(NULL);
    struct tm *tm_struct = localtime(&now);

    strftime(buffer, bufferSize, "%Y-%m-%d %H:%M:%S", tm_struct);
}
void getSystemUptime(char* buffer, int bufferSize) {
    int uptime = GetTickCount64();
    int hours = uptime / 3600000;
    int minutes = (uptime % 3600000) / 60000;

    snprintf(buffer, bufferSize, "%u hours and %u minutes.", hours, minutes);
}




