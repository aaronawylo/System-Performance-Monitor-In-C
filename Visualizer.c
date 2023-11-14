#ifndef CPU_UTILITIES_H
#define CPU_UTILITIES_H

void setupPdhQuery();
double getCurrentCpuUsage();

#endif // CPU_UTILITIES_H
#include <windows.h>
#include <pdh.h>
#include <stdio.h>
// Add additional headers for GUI
#include <stdlib.h>
#include <string.h>


LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

char szClassName[ ] = "CPUUsageWindowClass";

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                /* No menu */
    wincl.cbClsExtra = 0;                     /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                     /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl)) return 0;

    /* The class is registered, let's create the program */
    hwnd = CreateWindowEx(
            0,                   /* Extended possibilities for variation */
            szClassName,         /* Classname */
            "Awesome CPU Usage Monitor", /* Title Text */
            WS_OVERLAPPEDWINDOW, /* default window */
            CW_USEDEFAULT,       /* Windows decides the position */
            CW_USEDEFAULT,       /* where the window ends up on the screen */
            544,                 /* The programs width */
            375,                 /* and height in pixels */
            HWND_DESKTOP,        /* The window is a child-window to desktop */
            NULL,                /* No menu */
            hThisInstance,       /* Program Instance handler */
            NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    setupPdhQuery();  // Initialize PDH Query
    SetTimer(hwnd, 1, 500, NULL); // Set a timer to update every half second

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0)) {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Here you can add code to draw text or graphics
            char text[100];
            sprintf(text, "CPU Usage: %.2f%%", getCurrentCpuUsage());
            TextOut(hdc, 10, 10, text, strlen(text));

            EndPaint(hwnd, &ps);
        }
            break;
        case WM_TIMER: {
            InvalidateRect(hwnd, NULL, TRUE); // Force window to be redrawn
        }
            break;
        case WM_DESTROY:
            PostQuitMessage(0); /* send a WM_QUIT to the message queue */
            break;
        default: /* for messages that we don't deal with */
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}