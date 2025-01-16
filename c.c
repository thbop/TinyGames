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
#define OBJ_SCALE (vec3){10.0f, 10.0f, 10.0f}


void GeneratePortalMesh(mesh *m) {
    m->vertBufferSize = 6;
    m->vertBuffer     = (vec3*)malloc(sizeof(vec3)*6);
    m->vertBuffer[0]  = (vec3){ 0.0f,  0.7f,  0.0f};
    m->vertBuffer[1]  = (vec3){ 0.5f,  0.5f,  0.0f};
    m->vertBuffer[2]  = (vec3){ 0.5f, -0.5f,  0.0f};
    m->vertBuffer[3]  = (vec3){ 0.0f, -0.7f,  0.0f};
    m->vertBuffer[4]  = (vec3){-0.5f, -0.5f,  0.0f};
    m->vertBuffer[5]  = (vec3){-0.5f,  0.5f,  0.0f};

    m->quadBufferSize = 2;
    m->quadBuffer     = (quad*)malloc(sizeof(quad)*2);
    m->quadBuffer[0]  = (quad){ 5, 4, 3, 0 };
    m->quadBuffer[1]  = (quad){ 3, 2, 1, 0 };

}

mesh cube, portal;

obj SetPortalOnQuad(obj *o, int quadID, char c) {
    if ( quadID > o->m->quadBufferSize-1 ) return;
    vec3 q0 = toWorldSpace( o->m->vertBuffer[o->m->quadBuffer[quadID].id0], o );
    vec3 q1 = toWorldSpace( o->m->vertBuffer[o->m->quadBuffer[quadID].id1], o );
    vec3 q2 = toWorldSpace( o->m->vertBuffer[o->m->quadBuffer[quadID].id2], o );
    vec3 q3 = toWorldSpace( o->m->vertBuffer[o->m->quadBuffer[quadID].id3], o );

    vec3 n  = vec3Cross( vec3Sub(q0, q1), vec3Sub(q0, q3) );

    vec3 quadCenter = vec3MultiplyValue( vec3Add( vec3Add(q0, q1), vec3Add(q2, q3) ), 0.25f );
    return (obj){
        .m       = &portal,
        .origin  = quadCenter,
        .scale   = OBJ_SCALE,
        .rotation  = (vec3){0.0f, vec3GetRotation(n, 1), 0.0f},
        .fill    = c,
        .outline = c,
    };
}

obj *loadMap(const char *map, int *objBufferSize, int objBufferFreeSize) {
    *objBufferSize = 0;
    for ( int i = 0; i < MAP_AREA; i++ ) if ( map[i] == '#' ) (*objBufferSize)++;
    objBufferSize += objBufferFreeSize;

    obj *objs = (obj*)malloc(sizeof(obj)*(*objBufferSize));

    int objID = 0;

    for ( int j = 0; j < MAP_LENGTH; j++ ) {
        for ( int i = 0; i < MAP_WIDTH; i++ ) {
            switch ( map[j*MAP_WIDTH+i] ) {
                case '#':
                    objs[objID] = (obj){
                        .m         = &cube,
                        .origin    = (vec3){i*20.0f, 0.0f, j*20.0f},
                        .rotation  = (vec3){0.0f, 0.0f, 0.0f},
                        .scale     = OBJ_SCALE,
                        .outline   = '0',
                        .fill      = '.',
                    };
                    objID++;
                    break;
            }
        }
    }
    return objs;
}

int objSortComp(const void *a, const void *b) {
    return
        vec3LengthSquared( vec3Sub(camera.origin, ((obj*)a)->origin) ) <
        vec3LengthSquared( vec3Sub(camera.origin, ((obj*)b)->origin) );
}

int mainCRTStartup() {
    // Initialize screen
    Surface screen[SCREEN_SIZE] = {0};
    InitConsole();

    camera.focalLength = 64.0f;
    camera.origin = camera.rotation = (vec3){0.0f, 0.0f, 0.0f};

    GenerateCubeMesh(&cube);
    GeneratePortalMesh(&portal);

    const char map[MAP_AREA] =
        "................"
        "................"
        "................"
        "................"
        "................"
        "................"
        "................"
        "....#...#......."
        "....#..........."
        "....#..........."
        "....#..........."
        "....#...#......."
        "................"
        "................"
        "................"
        "................";

    int objBufferSize;
    obj *objs = loadMap(map, &objBufferSize, 2);

    objs[objBufferSize-1] = SetPortalOnQuad(objs+1, 4, '1'); // Set a portal on the left quad of a cube
    objs[objBufferSize-2] = SetPortalOnQuad(objs, 1, '2');   // Set a portal on the back quad of a cube

    unsigned char running = true;
    while ( running ) {
        if ( KeyDown(VK_ESCAPE) ) running = false;
        if ( KeyDown(VK_RIGHT) )  camera.rotation.y -= 0.05f;
        if ( KeyDown(VK_LEFT ) )  camera.rotation.y += 0.05f;
        if ( KeyDown('W') )       camera.origin = vec3Add(camera.origin, CameraForward(0.0f));
        if ( KeyDown('S') )       camera.origin = vec3Sub(camera.origin, CameraForward(0.0f));
        if ( KeyDown('A') )       camera.origin = vec3Add(camera.origin, CameraForward(PI/2));
        if ( KeyDown('D') )       camera.origin = vec3Sub(camera.origin, CameraForward(PI/2));

        // if ( KeyDown(VK_UP) ) camera.focalLength++;
        // if ( KeyDown(VK_DOWN) ) camera.focalLength--;

        ClearScreen(screen);

        qsort(objs, objBufferSize, sizeof(obj), objSortComp);
        for ( int i = 0; i < objBufferSize; i++ ) {
            if (objs+i != NULL) RenderObject(screen, objs+i);
        }

        // vec2 q = projectToCamera( vec3Add( vec3MultiplyValue( cube.vertBuffer[4], cube.scale ), cube.origin ) );
        // wsprintf(screen+SCREEN_WIDTH+1, "%d %d %d", (int)p.x, (int)p.y, (int)p.z);
        // DrawChar(screen, '@', (int)q.x, (int)q.y);

        UpdateConsole(screen); // Update screen
        Sleep(FRAMETIME);
    }
    free(objs);
    FreeMesh(&cube);
    FreeMesh(&portal);
    CloseConsole();
}