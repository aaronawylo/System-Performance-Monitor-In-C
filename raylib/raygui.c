#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "CPUUtilities.h"
#include "Memory.h"
#include "stdio.h"
#include "../Memory/Memory.h"

#define UPDATE_INTERVAL 1.0 // Update interval in seconds (e.g., update every 1 second)

int main() {
    const char* currentDirectory = GetWorkingDirectory();

    // Construct the full path to the theme file
    char themeFilePath[512];
    snprintf(themeFilePath, sizeof(themeFilePath), "%s/raygui/style_candy.rgs", currentDirectory);
    GuiLoadStyle(themeFilePath);

    InitWindow(800, 500, "Awesome CPU and Memory Usage Monitor");
    SetTargetFPS(8);  // Set FPS

    setupPdhQuery();  // Initialize your custom CPU and Memory monitoring

    // Set style for text boxes to white and increase font size
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, ColorToInt(RAYWHITE));
    GuiSetStyle(TEXTBOX, TEXT_SIZE, 20);  // Increase font size for text boxes

    // Define a custom blue color
    Color blueColor = (Color){0, 102, 255, 255}; // (R, G, B, A)

    double elapsedTime = 0.0; // Elapsed time since the last update
    double cpuUsage = 0.0;   // Current CPU usage

    // Main game loop
    while (!WindowShouldClose()) {
        // Update elapsed time
        double deltaTime = GetFrameTime();
        elapsedTime += deltaTime;

        // Update (handle input, update data, etc.)
        if (elapsedTime >= UPDATE_INTERVAL) {
            // Get the current CPU usage
            cpuUsage = getCurrentCpuUsage();

            // Reset the elapsed time
            elapsedTime = 0.0;
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Set text color to the custom blue color
        GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(blueColor));

        // Draw text label "CPU" in blue and align it with the meter
        DrawText("CPU:", 30, 25, 20, DARKBLUE);

        // Draw CPU usage as a meter
        Rectangle meterRect = { 90, 20, 200, 30 };
        DrawRectangleLines(meterRect.x, meterRect.y, meterRect.width, meterRect.height, DARKGRAY);

        // Calculate the width of the meter finger based on CPU usage
        float meterWidth = (float)(meterRect.width * (cpuUsage / 100.0));
        Rectangle meterFinger = { meterRect.x, meterRect.y, meterWidth, meterRect.height };
        DrawRectangleRec(meterFinger, GREEN);

        // Draw the CPU usage percentage
        char cpuText[100];
        sprintf(cpuText, "%.2f%%", cpuUsage);
        Rectangle textRect = { 300, 20, 120, 30 };
        GuiTextBox(textRect, cpuText, 100, false);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();  // Close window and OpenGL context

    return 0;
}
