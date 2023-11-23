#define RAYGUI_IMPLEMENTATION
#define UPDATE_INTERVAL 1.0
#define GRAPH_HISTORY_LENGTH 100

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


void DrawGraph(const int *values, int count, Rectangle bounds, Color color) {
    if (count > 1) {
        int maxValue = 0;
        for (int i = 0; i < count; i++) {
            if (values[i] > maxValue) maxValue = values[i];
        }

        for (int i = 1; i < count; i++) {
            Vector2 startPos = { bounds.x + (bounds.width / count) * (i - 1),
                                 bounds.y + bounds.height - (values[i - 1] / (float)maxValue) * bounds.height };
            Vector2 endPos = { bounds.x + (bounds.width / count) * i,
                               bounds.y + bounds.height - (values[i] / (float)maxValue) * bounds.height };
            DrawLineV(startPos, endPos, color);
        }
    }
}

void DrawMainContentGrid(Rectangle bounds, int spacing, int subdivs) {
    Vector2 mouseCell = {0, 0};
    GuiGrid(bounds, "", spacing, subdivs, &mouseCell);
    for (int i = 0; i < subdivs; i++) {
        Rectangle cell = {bounds.x + i * (bounds.width / subdivs), bounds.y, bounds.width / subdivs, bounds.height};
    }
}

int main() {
    //application window size
    int screenWidth = 1400;
    int screenHeight = 800;
    //initalize window
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
    double elapsedTime = 0.0;
    double cpuUsage = 0.0;
    double memoryUsage = 0;
    double diskUsage = 0.0;
    double totalNetworkTraffic = 0.0;

    //Inner window
    Rectangle panelRec = { 2, 2, (float)screenWidth-4, (float)screenHeight-4 };
    //close button
    int closeButtonSize = 20;
    // graph data
    int cpuHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int memoryHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int diskHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int networkHistory[GRAPH_HISTORY_LENGTH] = { 0 };
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
            totalNetworkTraffic = calculateTotalNetworkTraffic();
            PrintProcesses(processList, 100, &numProcesses);

            cpuHistory[historyIndex] = (int)(cpuUsage * 10);
            memoryHistory[historyIndex] = memoryUsage;
            diskHistory[historyIndex] = (int) diskUsage;
            networkHistory[historyIndex] = (int) totalNetworkTraffic;

            historyIndex = (historyIndex + 1) % GRAPH_HISTORY_LENGTH;
            elapsedTime = 0.0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //internal window initalization
        GuiWindowBox(panelRec, "Super Awesome Activiy Monitor");
        //title
        DrawText("SYSTEM MONITOR", 30, 70, 30, GetColor(0x3299b4ff));
        //GUI grid -
        Rectangle mainContentBounds = { 5, 5, screenWidth - 10, screenHeight- 10  }; // Adjust as needed
        int mainContentSpacing = 25;
        int mainContentSubdivs = 6;
//        DrawMainContentGrid(mainContentBounds, mainContentSpacing, mainContentSubdivs);

        //rectangles for graphs (not visible)
        Rectangle cpuGraphRec = { 50, 150, 300, 100 };
        DrawGraph(cpuHistory, GRAPH_HISTORY_LENGTH, cpuGraphRec, GetColor(0x81c0d0ff));
        Rectangle memoryGraphRec = { 50, 330, 300, 100 };
        DrawGraph(memoryHistory, GRAPH_HISTORY_LENGTH, memoryGraphRec, GetColor(0x81c0d0ff));
        Rectangle diskGraphRec = { 400, 150, 300, 100 };
        DrawGraph(memoryHistory, GRAPH_HISTORY_LENGTH, diskGraphRec, GetColor(0x81c0d0ff));
        Rectangle networkGraphRec = { 400, 330, 300, 100 };
        DrawGraph(memoryHistory, GRAPH_HISTORY_LENGTH, networkGraphRec, GetColor(0x81c0d0ff));
        //text for CPU
        char cpuText[100];
        sprintf(cpuText, "CPU Usage: %.2f%%", cpuUsage);
        DrawText(cpuText, 50, 260, 20, GetColor(0x3299b4ff));
        //text for memory
        char memoryText[100];
        sprintf(memoryText, "Memory Usage: %.2f%%", memoryUsage);
        DrawText(memoryText, 50, 440, 20, GetColor(0x3299b4ff));
        //text for disk
        char diskText[100];
        sprintf(cpuText, "Disk Usage: %.2f%%", diskUsage);
        DrawText(cpuText, 400, 260, 20, GetColor(0x3299b4ff));
        //text for network
        char networkText[100];
        sprintf(memoryText, "Total Network Traffic: %lf MB", totalNetworkTraffic);
        DrawText(memoryText, 400, 440, 20, GetColor(0x3299b4ff));

        //print processes
        for (int i = 0; i < numProcesses; i++) {
            char processInfo[512];
            sprintf(processInfo, "ID: %lu, Name: %s, Status: %s",
                    processList[i].processId,
                    processList[i].processName,
                    processList[i].status);

            GuiLabel((Rectangle){50, 500 + i * 20, 400, 20}, processInfo);
        }


        //footer boxes:
        int boxHeight = 35;
        int boxY = screenHeight - boxHeight - 5;
        //rectangle dimensions
        Rectangle box1 = { 5, boxY, 120, boxHeight };
        Rectangle box2 = { 5 + 120, boxY, 150, boxHeight };
        Rectangle box3 = { 5 + 120+150, boxY, 210, boxHeight };
        Rectangle box4 = { 5 + 120+150+210, boxY, 280, boxHeight };
        Rectangle box5 = { 5 + 120+150+210+280, boxY, 250, boxHeight };
        Rectangle box6 = { 5 + 120+150+210+280+250, boxY, screenWidth-165-150-300-200-200-6, boxHeight };
        //draw the rectangles
        DrawRectangleRec(box1, GetColor(0x024658ff));
        DrawRectangleRec(box2, GetColor(0x024658ff));
        DrawRectangleRec(box3, GetColor(0x024658ff));
        DrawRectangleRec(box4, GetColor(0x024658ff));
        DrawRectangleRec(box5, GetColor(0x024658ff));
        DrawRectangleRec(box6, GetColor(0x024658ff));
        //rectangle borders
        int borderWidth = 1;
        Color borderColor = GetColor(0x82cde0ff);
        DrawRectangleLinesEx(box1, borderWidth, borderColor);
        DrawRectangleLinesEx(box2, borderWidth, borderColor);
        DrawRectangleLinesEx(box3, borderWidth, borderColor);
        DrawRectangleLinesEx(box4, borderWidth, borderColor);
        DrawRectangleLinesEx(box5, borderWidth, borderColor);
        DrawRectangleLinesEx(box6, borderWidth, borderColor);
        //text in rectangles
        DrawText("USER: ", box1.x + 25, box1.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText(userName, box2.x + 25, box2.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText("DATE & TIME: ", box3.x + 25, box3.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText(dateTimeStr, box4.x + 25, box4.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText("SYSTEM UPTIME: ", box5.x + 25, box5.y + 8, 20, GetColor(0x51bfd3ff));
        DrawText(uptimeStr, box6.x + 25, box6.y + 8, 20, GetColor(0x51bfd3ff));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
