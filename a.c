#include <windows.h>
#include <string.h>

#define SCREEN_WIDTH 120 // Win 10 is probably 80
#define SCREEN_HEIGHT 30
#define SCREEN_SIZE SCREEN_WIDTH*SCREEN_HEIGHT
#define FPS 60
#define FRAMETIME (1000 / FPS)

#define true 1
#define false 0


struct {
    int y, vy;
} flappy;

#define ClearScreen() memset(screen, ' ', SCREEN_SIZE)

#define DrawChar(c, x, y) \
    screen[SCREEN_WIDTH*y+x] = c

int mainCRTStartup() {
    // Initialize screen
    unsigned char screen[SCREEN_SIZE] = { 0 };
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    DWORD dwBytesWritten;
    SetConsoleActiveScreenBuffer(hConsole);

    while ( true ) {
        ClearScreen();

        
        for ( int j = 0; j < 10; j++ ) {
            for ( int i = 0; i < 10; i++ ) {
                DrawChar( '#', i+10, j+10 );
            }
        }
        
        

        WriteConsoleOutputCharacter(hConsole, screen, SCREEN_SIZE, (COORD){0,0}, &dwBytesWritten); // Update screen
        Sleep(FRAMETIME);
    }
    

    CloseHandle(hConsole);
}