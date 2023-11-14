#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <stdio.h>

PDH_HQUERY cpuQuery;
PDH_HCOUNTER cpuTotal;

void setupPdhQuery() {
    PdhOpenQuery(NULL, 0, &cpuQuery);
    // This counter path works on English Windows systems. It may be different in other languages.
    PdhAddEnglishCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
}

double getCurrentCpuUsage() {
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}


int main() {
    setupPdhQuery();

    while (1) {
        printf("Current CPU Usage: %.2f%%\n", getCurrentCpuUsage());
        Sleep(1000); // Sample every second
    }

    // Close the query
    PdhCloseQuery(cpuQuery);

    return 0;
}
