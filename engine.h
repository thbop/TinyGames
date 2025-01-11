#ifndef ENGINE_H
#define ENGINE_H



// Application stuff
#define SCREEN_WIDTH       120 // Win 10 is probably 80
#define SCREEN_HEIGHT      30
#define HALF_SCREEN_WIDTH  (SCREEN_WIDTH>>1)
#define HALF_SCREEN_HEIGHT (SCREEN_HEIGHT>>1)
#define SCREEN_SIZE        (SCREEN_WIDTH*SCREEN_HEIGHT)

typedef unsigned char Surface;

#define InitConsole() \
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL); \
    DWORD dwBytesWritten; \
    SetConsoleActiveScreenBuffer(hConsole) \

#define UpdateConsole(surf) \
    WriteConsoleOutputCharacter(hConsole, surf, SCREEN_SIZE, (COORD){0,0}, &dwBytesWritten)

#define CloseConsole() \
    CloseHandle(hConsole)

// Keys
#define KeyDown(key)  GetAsyncKeyState(key)
#define K_SPACE       0x20

// Boolean
#define true          1
#define false         0

#define ClearScreen(screen) memset(screen, ' ', SCREEN_SIZE)

void DrawChar(Surface *screen, char c, int x, int y) {
    if ( 0 < x && x < SCREEN_WIDTH && 0 < y && y < SCREEN_HEIGHT )
        screen[SCREEN_WIDTH*y+x] = c;
}
    

void DrawRect( Surface *screen, char c, int x, int y, int width, int height ) {
    for ( int j = 0; j < height; j++ ) {
        for ( int i = 0; i < width; i++ ) {
            DrawChar(screen, c, x+i, y+j);
        }
    }
}

unsigned char CollisionPointRect( float x, float y, int rx, int ry, int rwidth, int rheight ) { // I would normally package vec2s and rects, but not here
    return rx <= x && x <= rx+rwidth &&
           ry <= y && y <= ry+rheight;
}




#endif