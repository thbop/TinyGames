#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "engine.h"
#include "line.h"
#include "3D.h"

// Game specific
#define FPS       60
#define FRAMETIME (1000 / FPS)

int mainCRTStartup() {
    // Initialize screen
    Surface screen[SCREEN_SIZE] = {0};
    InitConsole();

    camera.focalLength = 64.0f;
    camera.origin = camera.rotation = (vec3){0.0f, 0.0f, 0.0f};

    mesh cube;
    GenerateCubeMesh(&cube);
    cube.origin = (vec3){ 0.0f, 0.0f, 100.0f };
    cube.scale  = 16.0f;
    
    while ( true ) {
        if ( KeyDown(VK_RIGHT) ) camera.rotation.y -= 0.05f;
        if ( KeyDown(VK_LEFT ) ) camera.rotation.y += 0.05f;
        if ( KeyDown('W') )      camera.origin = vec3Add(camera.origin, CameraForward(0.0f));
        if ( KeyDown('S') )      camera.origin = vec3Sub(camera.origin, CameraForward(0.0f));
        if ( KeyDown('A') )      camera.origin = vec3Add(camera.origin, CameraForward(PI/2));
        if ( KeyDown('D') )      camera.origin = vec3Sub(camera.origin, CameraForward(PI/2));

        ClearScreen(screen);

        RenderMesh(screen, '#', &cube);

        
        // vec2 q = projectToCamera( vec3Add( vec3MultiplyValue( cube.vertBuffer[4], cube.scale ), cube.origin ) );
        // wsprintf(screen+SCREEN_WIDTH+1, "%d %d %d", (int)p.x, (int)p.y, (int)p.z);
        // DrawChar(screen, '@', (int)q.x, (int)q.y);

        UpdateConsole(screen); // Update screen
        Sleep(FRAMETIME);
    }
    
    FreeMesh(&cube);
    CloseConsole();
}