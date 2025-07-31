#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include <stdio.h>
#include <string.h>

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

#ifdef INCLUDE_GET_PRESSED_ASCII
#pragma optimize( "", off )
// Returns the last pressed key as ASCII
char GetPressedASCII() {
    BYTE keyboardState[256];
    WORD out[1];
    HKL layout = GetKeyboardLayout( 0 );

    for ( int vk = 0x08; vk <= 0xFE; vk++ ) {
        if ( GetAsyncKeyState( vk ) & 0x8000 ) {
            GetKeyboardState( keyboardState );
            UINT sc = MapVirtualKeyEx( vk, MAPVK_VK_TO_VSC, layout );
            int len = ToAscii( vk, sc, keyboardState, (LPWORD)out, 0 );
            if ( len == 1 && out[0] >= 0x20 && out[0] <= 0x7E )
                return out[0];
        }
    }

    return 0;
}
#pragma optimize( "", on )
#endif

// Boolean
#define true  1
#define false 0

#define ClearScreen(screen) memset(screen, ' ', SCREEN_SIZE)

char GetChar(Surface *screen, int x, int y) {
    if ( 0 < x && x < SCREEN_WIDTH && 0 < y && y < SCREEN_HEIGHT )
        return screen[SCREEN_WIDTH*y+x];
    return 0;
}

void DrawChar(Surface *screen, char c, int x, int y) {
    if ( 0 < x && x < SCREEN_WIDTH && 0 < y && y < SCREEN_HEIGHT )
        screen[SCREEN_WIDTH*y+x] = c;
}

void DrawMaskedChar(Surface *screen, char c, int x, int y, char mask) {
    if ( !mask ) DrawChar(screen, c, x, y);
    else if ( GetChar(screen, x, y) == mask ) DrawChar(screen, c, x, y);
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