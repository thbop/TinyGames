#define INCLUDE_GET_PRESSED_ASCII
#include "engine.h"

#define FPS         30
#define FRAMETIME   ( 1000 / FPS )

#define DAN_CHAR    '@'

// Screen and world
Surface
    screen[SCREEN_SIZE],
    world[SCREEN_SIZE];

// Dan's data
struct {
    int x, y;
    int health;
} dan;

// Draws the Heads Up Display
void DrawHUD() {
    wsprintf( screen + SCREEN_WIDTH + 1, "HEALTH: %u", dan.health );
}

// Main function
int mainCRTStartup() {
    // Initialize screen
    InitConsole();

    // Initialize world
    ClearScreen( world );

    // Initialize Dan
    dan.x = dan.y = dan.health = 10;


    while ( true ) {
        // Update
        // Movement
        if ( KeyDown( VK_LEFT ) )  dan.x -= 2;
        if ( KeyDown( VK_RIGHT ) ) dan.x += 2;
        if ( KeyDown( VK_UP ) )    dan.y--;
        if ( KeyDown( VK_DOWN ) )  dan.y++;

        // Get character places
        char ascii = GetPressedASCII();
        if ( ascii )
            world[dan.x + dan.y * SCREEN_WIDTH] = ascii;


        // Draw
        // Draw world
        memcpy( screen, world, SCREEN_SIZE );

        // Draw Dan
        DrawChar( screen, DAN_CHAR, dan.x, dan.y );

        // Draw HUD
        DrawHUD();

        UpdateConsole( screen );
        Sleep( FRAMETIME );
    }

    CloseConsole();
}