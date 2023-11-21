//
// Created by Mika on 2023-11-20.
// Stores current user and system info
//

#ifndef TERM__PROJECT__SYSTEM_MONITOR__CURRENTUSER_H
#define TERM__PROJECT__SYSTEM_MONITOR__CURRENTUSER_H

char* getCurrentUserName();
void getCurrentDateTime(char* buffer, int bufferSize);
void getSystemUptime(char* buffer, int bufferSize);

#endif //TERM__PROJECT__SYSTEM_MONITOR__CURRENTUSER_H
