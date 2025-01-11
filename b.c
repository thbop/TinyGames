#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "line.h"

// Game specific
#define FPS       60
#define FRAMETIME (1000 / FPS)



int mainCRTStartup() {
    // Initialize screen
    Surface screen[SCREEN_SIZE] = {0};
    InitConsole();

    
    while ( true ) {
        ClearScreen(screen);

        DrawLine(screen, '#', 0, 0, 45, 12);

        UpdateConsole(screen); // Update screen
        Sleep(FRAMETIME);
    }
    

    CloseConsole();
}