#define RAYGUI_IMPLEMENTATION
#define UPDATE_INTERVAL 1.0
#define GRAPH_HISTORY_LENGTH 100
#define NETWORK_HISTORY_LENGTH 100

#include "raylib.h"
#include "raygui.h"
#include "CPUUtilities.h"
#include "stdio.h"
#include "../Memory/Memory.h"
#include "style_cyber.h"
#include "../User/CurrentUser.h"
#include "../Data/ProcessingAndStorage.h"
#include "../Network/NetworkUtilities.h"
#include "../Process/Process.h"
#include "../Disk_IO/DiskIO_monitoring.h"


void DrawGraph(const int *values, int count, Rectangle bounds, Color color) {
    if (count > 1) {
        int maxValue = 100;
        for (int i = 0; i < count; i++) {
            if (values[i] > maxValue) maxValue = values[i];
        }

        for (int i = 1; i < count; i++) {
            Vector2 startPos = { bounds.x + (bounds.width / (float)count) * (float)(i - 1),
                                 bounds.y + bounds.height - ((float)values[i - 1] / (float)maxValue) * bounds.height };
            Vector2 endPos = { bounds.x + (bounds.width / (float)count) * (float)i,
                               bounds.y + bounds.height - ((float)values[i] / (float)maxValue) * bounds.height };
            DrawLineV(startPos, endPos, color);
        }
    }
}

void DrawProcessList(ProcessInfo *processList, int numProcesses, int startX, int startY, Font font, int fontSize) {
    // column widths
    int pidColumnWidth = 100;
    int nameColumnWidth = 400;
    const char* headers[] = {"PID", "Name"};
    int headerSpacing = 20;

    //draw headers
    for (int i = 0; i < 2; i++) {
        int columnX = startX + (i == 0 ? 0 : pidColumnWidth);
        DrawTextEx(font, headers[i], (Vector2){(float)columnX, (float)startY}, (float)fontSize, 1, GetColor(0x81c0d0ff));
    }

    int headerStartY = startY;
    startY += headerSpacing + 10;
    int outerBoxHeight = (numProcesses + 1) * (fontSize + 5) + 50;

    // draw the outer box
    Rectangle outerBox = {(float)startX - 25, (float)headerStartY - 15, (float)pidColumnWidth + (float)nameColumnWidth + 20, (float)outerBoxHeight + 30};
    DrawRectangleLinesEx(outerBox, 1, GetColor(0x024658ff));

    for (int i = 0; i < numProcesses; i++) {
        char pid[32], name[256];
        sprintf(pid, "%lu", processList[i].processId);
        sprintf(name, "%s", processList[i].processName);

        //draw the PID's and names
        DrawTextEx(font, pid, (Vector2){(float)startX, (float)startY + (float)i *(float)(fontSize + 5)}, (float)fontSize, 1, GetColor(0x3299b4ff));
        DrawTextEx(font, name, (Vector2){(float)startX + (float)pidColumnWidth, (float)startY + (float)i * (float)(fontSize + 5)}, (float)fontSize, 1, GetColor(0x3299b4ff));
    }
}


int main() {
    //application window size
    int screenWidth = 1500;
    int screenHeight = 900;
    //graph sizes
    int graphWidth = 230;
    int graphHeight = 130;

    int totalGraphHeight = 150;
    int totalGraphs = 3;
    int totalSpacing = screenHeight - (totalGraphHeight * totalGraphs);
    int spacingBetweenGraphs = totalSpacing / (totalGraphs + 1) + 50 ;

    InitWindow(screenWidth, screenHeight, "");
    //load the cyber GUI style
    GuiLoadStyleCyber();
    //set frame rate to 60 FPS
    SetTargetFPS(60);
    //get the current user system info: name, uptime, date/time
    char dateTimeStr[100];
    char* userName = getCurrentUserName();
    char uptimeStr[100];
    //set up for graphs
    setupPdhQuery();
    SetupSentQuery();
    SetupReceivedQuery();
    setupReadWriteQuery();

    double elapsedTime = 0.0;
    double cpuUsage = 0.0;
    double memoryUsage = 0.0;
    double diskUsage = 0.0;
    double diskRead = 0.0;
    double diskWrite = 0.0;
    double networkSent = 0.0;
    double networkReceived = 0.0;
    double totalNetworkSent = 0.0;
    double totalNetworkReceived = 0.0;


    // Setup for performance data log
    struct PerformanceData *currentData = malloc(sizeof(struct PerformanceData));
    if (currentData == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    //Inner window
    Rectangle panelRec = { 2, 2, (float)screenWidth-4, (float)screenHeight-4 };
    // graph data
    int cpuHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int memoryHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int diskHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    double networkSentHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    double networkReceivedHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int networkHistoryIndex = 0;
    int historyIndex = 0;

    ProcessInfo processList[100];
    int numProcesses = 0;

    while (!WindowShouldClose()) {
        //keep track of time for graph
        double deltaTime = GetFrameTime();
        elapsedTime += deltaTime;

        //get user info
        getCurrentDateTime(dateTimeStr, sizeof(dateTimeStr));
        getSystemUptime(uptimeStr, sizeof(uptimeStr));

        if (elapsedTime >= UPDATE_INTERVAL) {
            cpuUsage = getCurrentCpuUsage();
            memoryUsage = getMemoryUsagePercentage();
            diskUsage = calculateDiskUsagePercentage();
            diskRead = getDiskRead();
            diskWrite = getDiskWrite();

            double currentNetworkSent = GetNetworkSent();
            double currentNetworkReceived = GetNetworkReceived();

            totalNetworkSent += currentNetworkSent * UPDATE_INTERVAL;
            totalNetworkReceived += currentNetworkReceived * UPDATE_INTERVAL;

            networkSent = currentNetworkSent;
            networkReceived = currentNetworkReceived;

            PrintProcesses(processList, 50, &numProcesses);

            // Retrieve data
            strcpy(currentData->timestamp, dateTimeStr);
            currentData->cpu = cpuUsage;
            currentData->memory = memoryUsage;
            currentData->diskUsage = diskUsage;
            currentData->networkSent = networkSent;
            currentData->networkReceived = networkReceived;

            saveData(currentData, "performance_data.txt");

            cpuHistory[historyIndex] = (int) (cpuUsage * 10);
            memoryHistory[historyIndex] = (int) memoryUsage;
            diskHistory[historyIndex] = (int) diskUsage;
            networkSentHistory[networkHistoryIndex] = (int) networkSent;
            networkReceivedHistory[networkHistoryIndex] = (int) networkReceived;

            networkHistoryIndex = (networkHistoryIndex + 1) % NETWORK_HISTORY_LENGTH;
            historyIndex = (historyIndex + 1) % GRAPH_HISTORY_LENGTH;
            elapsedTime = 0.0;
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);
        //internal window initalization
        GuiWindowBox(panelRec, "Super Awesome Activiy Monitor");
        //title
        DrawText("SYSTEM MONITOR", 180, 80, 45, GetColor(0x3299b4ff));
        GuiDrawIcon(ICON_WAVE_TRIANGULAR, 80, 65, 5, GetColor(0x3299b4ff));
        GuiDrawIcon(ICON_WAVE_TRIANGULAR, 615, 65, 5, GetColor(0x3299b4ff));
        int fontSize = 20;

        //cpu section
        Rectangle cpuGraphRec = {90, (float)spacingBetweenGraphs + 50 + 20, (float)graphWidth, 130};
        DrawText("CPU", 80, spacingBetweenGraphs, 25, GetColor(0x3299b4ff));
        DrawGraph(cpuHistory, GRAPH_HISTORY_LENGTH, cpuGraphRec, GetColor(0x3299b4ff));
        char cpuText[100];
        sprintf(cpuText, "Usage: %.1f%%", cpuUsage);
        DrawText(cpuText, 90,(int)cpuGraphRec.y + graphHeight + 20, fontSize, GetColor(0x81c0d0ff));
        Rectangle outerBox = {70, cpuGraphRec.y-20, 380, 130 + (float)fontSize + 60};
        DrawRectangleLinesEx(outerBox, 1, GetColor(0x024658ff));


        //disk section
        Rectangle diskGraphRec = {90, cpuGraphRec.y + cpuGraphRec.height + (float)spacingBetweenGraphs - 15, (float)graphWidth, (float)graphHeight};
        DrawText("Disk", 80, (int)diskGraphRec.y - 25, 25, GetColor(0x3299b4ff));
        DrawGraph(diskHistory, GRAPH_HISTORY_LENGTH, diskGraphRec, GetColor(0x3299b4ff));
        char diskText[100];
        sprintf(diskText, "Usage: %.1f%%", diskUsage);
        DrawText(diskText, (int)(diskGraphRec.x), (int)diskGraphRec.y + 10 + (int)diskGraphRec.height + 25, 20, GetColor(0x81c0d0ff));
        int diskInfoY = (int)diskGraphRec.y + (int)diskGraphRec.height + 60; // Adjust Y-coordinate
        char diskReadText[100];
        sprintf(diskReadText, "Read Bytes: %.1f B", diskRead);
        DrawText(diskReadText, (int)diskGraphRec.x, diskInfoY + 20, 20, GetColor(0x81c0d0ff));
        char diskWriteText[100];
        sprintf(diskWriteText, "Write Bytes: %.1f B", diskWrite);
        DrawText(diskWriteText, 100, diskInfoY + 50, 20, GetColor(0x81c0d0ff));
        Rectangle diskOuterBox = {
                70,
                diskGraphRec.y + 20,
                380,
                diskGraphRec.height + ((float)diskInfoY - diskGraphRec.y) - 25
        };

        DrawRectangleLinesEx(diskOuterBox, 1, GetColor(0x024658ff));


        //memory section
        Rectangle memoryGraphRec = {520, (float)spacingBetweenGraphs + 50 + 20, (float)graphWidth, 130};
        DrawText("Memory", 510, spacingBetweenGraphs, 25, GetColor(0x3299b4ff));
        DrawGraph(memoryHistory, GRAPH_HISTORY_LENGTH, memoryGraphRec, GetColor(0x3299b4ff));
        char memoryText[100];
        sprintf(memoryText, "Usage: %.1f%%", memoryUsage);
        DrawText(memoryText, 520, (int)cpuGraphRec.y + graphHeight + 20, 20, GetColor(0x81c0d0ff));
        // Defining the outer box for the memory graph
        Rectangle memoryOuterBox = {
                500,
                cpuGraphRec.y-20,
                380,
                130 + (float)fontSize + 60
        };
        DrawRectangleLinesEx(memoryOuterBox, 1, GetColor(0x024658ff)); // Drawing the rectangle


        //network section
        Vector2 position;
        position = (Vector2){510, 530};
        DrawText("Network", (int)position.x + 5, (int)diskGraphRec.y - 25, fontSize + 5, GetColor(0x3299b4ff));
        GuiDrawIcon(ICON_ARROW_UP_FILL, (int)position.x + 15, (int)position.y + 30, 2, GetColor(0x3299b4ff));
        DrawText(TextFormat("Sending: %.1f B/s", networkSent), (int)position.x + 70, (int)position.y + 35, fontSize, GetColor(0x81c0d0ff));
        position.y += 40;
        DrawText(TextFormat("Total Sent: %.1fB", totalNetworkSent),(int) position.x + 70, (int)position.y + 20, fontSize,GetColor(0x81c0d0ff));
        position.y += 40;
        GuiDrawIcon(ICON_ARROW_DOWN_FILL, (int)position.x + 15, (int)position.y + 30, 2, GetColor(0x3299b4ff));
        DrawText(TextFormat("Receiving: %.1f B/s", networkReceived), (int)position.x + 70, (int)position.y + 35, fontSize, GetColor(0x81c0d0ff));
        position.y += 40;
        DrawText(TextFormat("Total Received: %.1f B", totalNetworkReceived), (int)position.x + 70, (int)position.y + 20, fontSize, GetColor(0x81c0d0ff));
        Rectangle networkOuterBox = {
                500,
                diskGraphRec.y + 20,
                380,
                position.y + 60 - (diskGraphRec.y - 25)
        };
        DrawRectangleLinesEx(networkOuterBox, 1, GetColor(0x024658ff));


        //applications section:
        Font customFont = GetFontDefault();
        char applicationsText[100];
        sprintf(applicationsText, "Applications");
        DrawText(applicationsText, 940, spacingBetweenGraphs, 25, GetColor(0x3299b4ff));
        DrawProcessList(processList, numProcesses, 950, spacingBetweenGraphs + 60, customFont, fontSize);


        //footer boxes:
        int boxHeight = 35;
        int boxY = screenHeight - boxHeight - 5;

        Rectangle box1 = { 5, (float)boxY, 120, (float)boxHeight };
        Rectangle box2 = { 5 + 120, (float)boxY, 150, (float)boxHeight };
        Rectangle box3 = { 5 + 120+150, (float)boxY, 210, (float)boxHeight };
        Rectangle box4 = { 5 + 120+150+210, (float)boxY, 280, (float)boxHeight };
        Rectangle box5 = { 5 + 120+150+210+280, (float)boxY, 250, (float)boxHeight };
        Rectangle box6 = { 5 + 120+150+210+280+250, (float)boxY, (float)screenWidth-165-150-300-200-200-6, (float)boxHeight };

        DrawRectangleRec(box1, GetColor(0x024658ff));
        DrawRectangleRec(box2, GetColor(0x024658ff));
        DrawRectangleRec(box3, GetColor(0x024658ff));
        DrawRectangleRec(box4, GetColor(0x024658ff));
        DrawRectangleRec(box5, GetColor(0x024658ff));
        DrawRectangleRec(box6, GetColor(0x024658ff));

        int borderWidth = 1;
        Color borderColor = GetColor(0x82cde0ff);
        DrawRectangleLinesEx(box1, (float)borderWidth, borderColor);
        DrawRectangleLinesEx(box2, (float)borderWidth, borderColor);
        DrawRectangleLinesEx(box3, (float)borderWidth, borderColor);
        DrawRectangleLinesEx(box4,(float) borderWidth, borderColor);
        DrawRectangleLinesEx(box5, (float)borderWidth, borderColor);
        DrawRectangleLinesEx(box6, (float)borderWidth, borderColor);

        DrawText("USER: ", (int)box1.x + 25, (int)box1.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText(userName, (int)box2.x + 25, (int)box2.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText("DATE & TIME: ", (int)box3.x + 25, (int)box3.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText(dateTimeStr, (int)box4.x + 25, (int)box4.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText("SYSTEM UPTIME: ", (int)box5.x + 25, (int)box5.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText(uptimeStr, (int)box6.x + 25, (int)box6.y + 8, 20, GetColor(0x51bfd3ff));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
