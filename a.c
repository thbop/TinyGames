#include <windows.h>
#include <string.h>

// Application stuff
#define SCREEN_WIDTH         120 // Win 10 is probably 80
#define SCREEN_HEIGHT        30
#define SCREEN_SIZE          SCREEN_WIDTH*SCREEN_HEIGHT
#define FPS                  60
#define FRAMETIME            (1000 / FPS)

// Keys
#define KeyDown(key)         GetAsyncKeyState(key)
#define K_SPACE              0x20

// Boolean
#define true                 1
#define false                0

#define ClearScreen() memset(screen, ' ', SCREEN_SIZE)

void DrawChar(unsigned char *screen, char c, int x, int y) {
    if ( 0 < x && x < SCREEN_WIDTH && 0 < y && y < SCREEN_HEIGHT )
        screen[SCREEN_WIDTH*y+x] = c;
}
    

void DrawRect( unsigned char *screen, char c, int x, int y, int width, int height ) {
    // for ( int j = 0; j < height; j++ ) {
    //     for ( int i = 0; i < width; i++ ) {
    //         DrawChar(c, x+i, y+j);
    //     }
    // }
    for ( int j = 0; j < height; j++ ) {
        for ( int i = 0; i < width; i++ ) {
            DrawChar(screen, '#', x+i, y+j);
        }
    }
}

// Game specific
#define FLAPPY_GLYPH         '%'
#define FLAPPY_X             30
#define FLAPPY_FLAP_ACC      0.5f;
#define FLAPPY_JUMP_COOLDOWN 3
#define GRAVITY              0.1f;

#define PIPE_GLYPH           '#'
#define NUM_OF_PIPES         10
#define PIPE_X_SPACING       (SCREEN_WIDTH / NUM_OF_PIPES)
#define PIPE_GAP_SIZE        4
#define PIPE_WIDTH           3
#define PIPE_SPEED           0.24f;



struct {
    float y, vy;
    int cooldown;
} flappy;

typedef struct {
    float x;
    int openingY;
} pipe;

void DrawPipe(unsigned char *screen, pipe p) {
    DrawRect(screen, PIPE_GLYPH, (int)p.x, 0, PIPE_WIDTH, 12); // 12 should be p.openingY
    DrawRect(screen, PIPE_GLYPH, (int)p.x, (12+PIPE_GAP_SIZE), PIPE_WIDTH, SCREEN_HEIGHT-(12+PIPE_GAP_SIZE));

}


int mainCRTStartup() {
    // Initialize screen
    unsigned char screen[SCREEN_SIZE] = { 0 };
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    DWORD dwBytesWritten;
    SetConsoleActiveScreenBuffer(hConsole);

    // Flappy
    flappy.vy = 0.0f;
    flappy.cooldown = 0;
    flappy.y = SCREEN_HEIGHT*0.5f;

    // Pipes
    pipe pipes[NUM_OF_PIPES];
    for ( int i = 1; i < NUM_OF_PIPES; i++ )
        pipes[i].x = -1.0f;
    int nextPipe = 0;
    pipes[0].x = SCREEN_WIDTH-10;

    unsigned char running = true;
    while ( running ) {
        // Flappy
        flappy.vy += GRAVITY;
        if ( flappy.vy > 1.0f ) flappy.vy = 1.0f;
        if ( KeyDown(K_SPACE) && !flappy.cooldown ) {
            flappy.vy = -FLAPPY_FLAP_ACC;
            flappy.cooldown = FLAPPY_JUMP_COOLDOWN;
        }
        if ( flappy.cooldown > 0 ) flappy.cooldown--;

        flappy.y += flappy.vy;

        ClearScreen();

        // Pipes
        for ( int i = 0; i < NUM_OF_PIPES; i++ ) {
            if ( pipes[i].x > 0.0f ) {
                pipes[i].x -= PIPE_SPEED;
                DrawPipe(screen, pipes[i]);
            }
        }
        if ( pipes[nextPipe].x < SCREEN_WIDTH-PIPE_X_SPACING ) {
            if ( nextPipe < NUM_OF_PIPES-1 ) nextPipe++;
            else                             nextPipe = 0;

            pipes[nextPipe].x = SCREEN_WIDTH;
        }

        // Flappy
        DrawChar( screen, FLAPPY_GLYPH, FLAPPY_X, (int)flappy.y );

        WriteConsoleOutputCharacter(hConsole, screen, SCREEN_SIZE, (COORD){0,0}, &dwBytesWritten); // Update screen
        Sleep(FRAMETIME);
    }
    

    CloseHandle(hConsole);
}