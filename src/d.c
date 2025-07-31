#include "engine.h"

#define FPS         45
#define FRAMETIME   ( 1000 / FPS )

// Dan's data
struct {
    int x, y;
} dan;

// Main function
int mainCRTStartup() {
    // Initialize screen
    static Surface screen[SCREEN_SIZE] = { 0 };
    InitConsole();

    // Initialize Dan
    dan.x = dan.y = 10;

    while ( true ) {
        // Update


        // Draw
        ClearScreen( screen );

        DrawChar( screen, '#', dan.x, dan.y );

        UpdateConsole( screen );
        Sleep( FRAMETIME );
    }

    CloseConsole();
}