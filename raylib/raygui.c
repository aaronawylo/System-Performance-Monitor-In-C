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

void DrawGraph(int *values, int count, Rectangle bounds, Color color) {
    if (count > 1) {
        int maxValue = 0;
        for (int i = 0; i < count; i++) {
            if (values[i] > maxValue) maxValue = values[i];
        }

        for (int i = 1; i < count; i++) {
            Vector2 startPos = { bounds.x + (bounds.width / count) * (i - 1), bounds.y + bounds.height - (values[i - 1] / (float)maxValue) * bounds.height };
            Vector2 endPos = { bounds.x + (bounds.width / count) * i, bounds.y + bounds.height - (values[i] / (float)maxValue) * bounds.height };
            DrawLineV(startPos, endPos, color);
        }
    }
}

int main() {
    //application window size
    int screenWidth = 1280;
    int screenHeight = 720;
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
    //set up for CPU & Memory graphs
    setupPdhQuery();
    double elapsedTime = 0.0;
    double cpuUsage = 0.0;
    int memoryUsage = 0;
    //Inner window
    Rectangle panelRec = { 5, 5, (float)screenWidth - 10, (float)screenHeight - 10 };

    // Graph data
    int cpuHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int memoryHistory[GRAPH_HISTORY_LENGTH] = { 0 };
    int historyIndex = 0;

    while (!WindowShouldClose()) {
        //keep track of time for graph
        double deltaTime = GetFrameTime();
        elapsedTime += deltaTime;

        //get user info
        getCurrentDateTime(dateTimeStr, sizeof(dateTimeStr));
        getSystemUptime(uptimeStr, sizeof(uptimeStr));

        if (elapsedTime >= UPDATE_INTERVAL) {
            cpuUsage = getCurrentCpuUsage();
            memoryUsage = getMemoryUsage();

            cpuHistory[historyIndex] = (int)(cpuUsage * 10);
            memoryHistory[historyIndex] = memoryUsage;

            historyIndex = (historyIndex + 1) % GRAPH_HISTORY_LENGTH;
            elapsedTime = 0.0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //internal window initalization
        GuiPanel(panelRec, "Super Awesome System Activity Monitor");
        //title
        DrawText("SYSTEM MONITOR", 30, 70, 30, GetColor(0x3299b4ff));
        //rectangles for graphs (not visible)
        Rectangle cpuGraphRec = { 50, 150, 300, 100 };
        DrawGraph(cpuHistory, GRAPH_HISTORY_LENGTH, cpuGraphRec, GetColor(0x81c0d0ff));
        Rectangle memoryGraphRec = { 50, 330, 300, 100 };
        DrawGraph(memoryHistory, GRAPH_HISTORY_LENGTH, memoryGraphRec, GetColor(0x81c0d0ff));
        //text for CPU
        char cpuText[100];
        sprintf(cpuText, "CPU Usage: %.2f%%", cpuUsage);
        DrawText(cpuText, 50, 260, 20, GetColor(0x3299b4ff));
        //text for memory
        char memoryText[100];
        sprintf(memoryText, "Memory Usage: %d MB", memoryUsage);
        DrawText(memoryText, 50, 440, 20, GetColor(0x3299b4ff));
        //footer boxes:
        int boxHeight = 35;
        int boxY = screenHeight - boxHeight - 5;
        //rectangle dimensions
        Rectangle box1 = { 10, boxY, 165, boxHeight };
        Rectangle box2 = { 10 + 165, boxY, 150, boxHeight };
        Rectangle box3 = { 10 + 165+150, boxY, 300, boxHeight };
        Rectangle box4 = { 10 + 165+150+300, boxY, 200, boxHeight };
        Rectangle box5 = { 10 + 165+150+300+200, boxY, 200, boxHeight };
        Rectangle box6 = { 10 + 165+150+300+200+200, boxY, screenWidth-165-150-300-200-200-20, boxHeight };
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
        DrawText("Current User: ", box1.x + 12, box1.y + 8, 18, GetColor(0x51bfd3ff));
        DrawText(userName, box2.x + 15, box2.y + 8, 18, GetColor(0x51bfd3ff));
        DrawText("CURRENT DATE & TIME: ", box3.x + 40, box3.y + 8, 18, GetColor(0x51bfd3ff));
        DrawText(dateTimeStr, box4.x + 22, box4.y + 8, 18, GetColor(0x51bfd3ff));
        DrawText("SYSTEM UPTIME: ", box5.x + 22, box5.y + 8, 18, GetColor(0x51bfd3ff));
        DrawText(uptimeStr, box6.x + 22, box6.y + 8, 18, GetColor(0x51bfd3ff));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
