#define INCLUDE_GET_PRESSED_ASCII
#include "engine.h"

#ifdef __GNUC__
void __main() {}


void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) *p++ = (unsigned char)c;
    return s;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) *d++ = *s++;
    return dest;
}
#endif

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
    volatile int blocks;
} dan;

// Draws the Heads Up Display
void DrawHUD() {
    wsprintf( screen + SCREEN_WIDTH + 1, "HEALTH: %u", dan.health );
    wsprintf( screen + SCREEN_WIDTH * 2 + 1, "BLOCKS: %u", dan.blocks );
}


// Main function
int main() {
    // Initialize screen
    InitConsole();

    // Initialize world
    ClearScreen( world );

    // Initialize Dan
    dan.x = dan.y = dan.health = 10;
    dan.blocks = 100;


    while ( true ) {
        // Update
        // Movement
        if ( KeyDown( VK_LEFT ) )  dan.x -= 2;
        if ( KeyDown( VK_RIGHT ) ) dan.x += 2;
        if ( KeyDown( VK_UP ) )    dan.y--;
        if ( KeyDown( VK_DOWN ) )  dan.y++;

        // Get character places
        volatile char ascii = GetPressedASCII();
        if ( ( '0' <= ascii && ascii <= '9' ) && dan.blocks ) {
            if ( GetChar( world, dan.x, dan.y ) != ascii ) {
                dan.blocks -= ascii - '0';
                if ( dan.blocks < 0 )
                dan.blocks = 0;
            }
            DrawChar( world, ascii, dan.x, dan.y );
        }


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