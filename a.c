#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"

// Game specific
#define FPS             45
#define FRAMETIME       (1000 / FPS)
#define GRAVITY         0.1f
#define RESET_DELAY     500

#define FLAPPY_GLYPH    '%'
#define FLAPPY_X        30
#define FLAPPY_FLAP_ACC 0.5f

#define PIPE_GLYPH      '#'
#define NUM_OF_PIPES    5
#define PIPE_X_SPACING  (SCREEN_WIDTH / NUM_OF_PIPES)
#define PIPE_GAP_SIZE   5
#define PIPE_TD_PADDING 10                            // The padding between possible gaps and the ceiling/floor            
#define PIPE_WIDTH      3
#define PIPE_SPEED      0.24f;



struct {
    float y, vy;
} flappy;

void ResetFlappy() {
    flappy.vy = 0.0f;
    flappy.y = SCREEN_HEIGHT*0.5f;
}

typedef struct {
    float x;
    int openingY;
} pipe;

// Cheeky, ik
#define GetPipeTopRect(p) (int)p.x, 0, PIPE_WIDTH, p.openingY
#define GetPipeBottomRect(p) (int)p.x, (p.openingY+PIPE_GAP_SIZE), PIPE_WIDTH, SCREEN_HEIGHT-(p.openingY+PIPE_GAP_SIZE)

void DrawPipe(unsigned char *screen, pipe p) {
    DrawRect(screen, PIPE_GLYPH, GetPipeTopRect(p));
    DrawRect(screen, PIPE_GLYPH, GetPipeBottomRect(p));

}

void NewPipe( pipe *p ) {
    p->x = SCREEN_WIDTH;
    p->openingY = PIPE_TD_PADDING + rand() % (SCREEN_HEIGHT-PIPE_TD_PADDING-PIPE_GAP_SIZE);
}

void ResetPipes( pipe *pipes, int *nextPipe ) {
    for ( int i = 1; i < NUM_OF_PIPES; i++ )
        pipes[i].x = -1.0f;
    *nextPipe = 0;
    NewPipe(pipes); // Essentially &pipes[0]
}

void Reset( pipe *pipes, int *nextPipe, unsigned int *score ) {
    Sleep(RESET_DELAY);
    ResetFlappy();
    ResetPipes(pipes, nextPipe);

    *score = 0;
}

int mainCRTStartup() {
    // Initialize screen
    Surface screen[SCREEN_SIZE] = {0};
    InitConsole();

    pipe pipes[NUM_OF_PIPES];
    int nextPipe;

    unsigned int score;

    Reset(pipes, &nextPipe, &score);
    
    while ( true ) {
        // Flappy
        flappy.vy += GRAVITY;
        if ( flappy.vy > 1.0f ) flappy.vy = 1.0f;
        if ( KeyDown(VK_SPACE) ) {
            flappy.vy = -FLAPPY_FLAP_ACC;
        }

        flappy.y += flappy.vy;
        if ( flappy.y <= 0 ) flappy.y = 0;

        if ( flappy.y >= SCREEN_HEIGHT )
            Reset(pipes, &nextPipe, &score);

        ClearScreen(screen);

        // Pipes
        for ( int i = 0; i < NUM_OF_PIPES; i++ ) {
            // Pipe movement and drawing
            if ( pipes[i].x > 0.0f ) {
                pipes[i].x -= PIPE_SPEED;
                DrawPipe(screen, pipes[i]);

                if ( (int)pipes[i].x == FLAPPY_X ) score++; // Not the best, but should work
            }
            // Check collisions
            if (
                CollisionPointRect(FLAPPY_X, flappy.y, GetPipeTopRect(pipes[i])) ||
                CollisionPointRect(FLAPPY_X, flappy.y, GetPipeBottomRect(pipes[i]))
            ) {
                Reset(pipes, &nextPipe, &score);
            }
        }
        // Cycle pipes
        if ( pipes[nextPipe].x < SCREEN_WIDTH-PIPE_X_SPACING ) {
            if ( nextPipe < NUM_OF_PIPES-1 ) nextPipe++;
            else                             nextPipe = 0;

            NewPipe(pipes+nextPipe); // Essentially &pipes[nextPipe]
        }

        // Flappy
        DrawChar( screen, FLAPPY_GLYPH, FLAPPY_X, (int)flappy.y );
        wsprintf(screen+SCREEN_WIDTH+1, "SCORE: %u", score); // Unsafe but cool

        UpdateConsole(screen); // Update screen
        Sleep(FRAMETIME);
    }
    

    CloseConsole();
}