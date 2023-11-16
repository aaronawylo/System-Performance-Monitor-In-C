#include <stdio.h>
#include <windows.h>

void displaySystemUptime() {
    unsigned int uptime = GetTickCount64();
    unsigned int hours = uptime / 3600000;
    unsigned int minutes = (uptime % 3600000) / 60000;

    printf("The system has been up for %u hours and %u minutes.\n", hours, minutes);
}

// int main() {
//     displaySystemUptime();
//     return 0;
// }
