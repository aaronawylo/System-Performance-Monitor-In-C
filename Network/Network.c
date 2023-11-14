//
// Created by Cameron on 11/14/2023.
//
#include <windows.h>
#include <pdh.h>
#include <stdio.h>
#include "NetworkUtilities.h"

#pragma comment(lib, "pdh.lib")

PDH_HQUERY sentQuery, receivedQuery;
PDH_HCOUNTER networkCountSent, networkCountReceived;
PDH_FMT_COUNTERVALUE valueSent, valueReceived;

void SetupSentQuery() {
    PDH_STATUS status = PdhOpenQuery(NULL, 0, &sentQuery);
    status = PdhAddEnglishCounter(sentQuery, "\\Network Interface(*)\\Bytes Sent/sec", 0, &networkCountSent);
    status = PdhCollectQueryData(sentQuery);
}

void SetupReceivedQuery() {
    PDH_STATUS status = PdhOpenQuery(NULL, 0, &receivedQuery);
    status = PdhAddEnglishCounter(receivedQuery, "\\Network Interface(*)\\Bytes Received/sec", 0, &networkCountReceived);
    status = PdhCollectQueryData(receivedQuery);
}

double GetNetworkSent() {
    PDH_STATUS status = PdhCollectQueryData(sentQuery);
    status = PdhGetFormattedCounterValue(networkCountSent, PDH_FMT_DOUBLE, NULL, &valueSent);
    return valueSent.doubleValue;
}

double GetNetworkReceived() {
    PDH_STATUS status = PdhCollectQueryData(receivedQuery);
    status = PdhGetFormattedCounterValue(networkCountReceived, PDH_FMT_DOUBLE, NULL, &valueReceived);
    return valueReceived.doubleValue;
}