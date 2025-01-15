#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "vec23.h"
#include "shapes.h"
#include "3D.h"

// Game specific
#define FPS       60
#define FRAMETIME (1000 / FPS)
#define MAP_WIDTH 16
#define MAP_LENGTH 16
#define MAP_AREA (MAP_WIDTH*MAP_LENGTH)


mesh cube; //, quadM;

obj *loadMap(const char *map, int *objBufferSize) {
    *objBufferSize = 0;
    for ( int i = 0; i < MAP_AREA; i++ ) if ( map[i] == '#' ) (*objBufferSize)++;

    obj *objs = (obj*)malloc(sizeof(obj)*(*objBufferSize));

    int objID = 0;

    for ( int j = 0; j < MAP_LENGTH; j++ ) {
        for ( int i = 0; i < MAP_WIDTH; i++ ) {
            switch ( map[j*MAP_WIDTH+i] ) {
                case '#':
                    objs[objID] = (obj){
                        .m         = &cube,
                        .origin    = (vec3){i*20.0f, 0.0f, j*20.0f},
                        .scale     = (vec3){10.0f, 10.0f, 10.0f},
                        // .rotation  = (vec3){0.0f, 0.0f, 0.0f}
                    };
                    objID++;
                    break;
            }
        }
    }
    return objs;
}

int mainCRTStartup() {
    // Initialize screen
    Surface screen[SCREEN_SIZE] = {0};
    InitConsole();

    camera.focalLength = 64.0f;
    camera.origin = camera.rotation = (vec3){0.0f, 0.0f, 0.0f};

    GenerateCubeMesh(&cube);
    // GenerateQuadMesh(&quadM);

    const char map[MAP_AREA] =
        "................"
        "................"
        "................"
        "................"
        "................"
        "................"
        "................"
        "....#...#......."
        "................"
        "......#........."
        "................"
        "....#...#......."
        "................"
        "................"
        "................"
        "................";

    int objBufferSize;
    obj *objs = loadMap(map, &objBufferSize);

    objs[2].scale = (vec3){5.0f, 5.0f, 5.0f};
    
    while ( true ) {
        if ( KeyDown(VK_RIGHT) ) camera.rotation.y -= 0.05f;
        if ( KeyDown(VK_LEFT ) ) camera.rotation.y += 0.05f;
        if ( KeyDown('W') )      camera.origin = vec3Add(camera.origin, CameraForward(0.0f));
        if ( KeyDown('S') )      camera.origin = vec3Sub(camera.origin, CameraForward(0.0f));
        if ( KeyDown('A') )      camera.origin = vec3Add(camera.origin, CameraForward(PI/2));
        if ( KeyDown('D') )      camera.origin = vec3Sub(camera.origin, CameraForward(PI/2));

        // if ( KeyDown(VK_UP) ) camera.focalLength++;
        // if ( KeyDown(VK_DOWN) ) camera.focalLength--;

        ClearScreen(screen);


        for ( int i = 0; i < objBufferSize; i++ ) {
            RenderObject(screen, '#', &objs[i]);
        }

            

        
        // vec2 q = projectToCamera( vec3Add( vec3MultiplyValue( cube.vertBuffer[4], cube.scale ), cube.origin ) );
        // wsprintf(screen+SCREEN_WIDTH+1, "%d %d %d", (int)p.x, (int)p.y, (int)p.z);
        // DrawChar(screen, '@', (int)q.x, (int)q.y);

        UpdateConsole(screen); // Update screen
        Sleep(FRAMETIME);
    }
    free(objs);
    FreeMesh(&cube);
    // FreeMesh(&quadM);
    CloseConsole();
}