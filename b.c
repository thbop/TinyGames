#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "line.h"

// Game specific
#define FPS       60
#define FRAMETIME (1000 / FPS)

typedef struct {
    float x, y;
} vec2;

#define DrawLineV(screen, c, l0, l1) \
    DrawLine(screen, c, (int)l0.x, (int)l0.y, (int)l1.x, (int)l1.y )

typedef struct {
    float x, y, z;
} vec3;

vec3 vec3MultiplyValue(vec3 p, float v) {
    return (vec3){ p.x * v, p.y * v, p.z * v };
}

vec3 vec3Add(vec3 p, vec3 q) {
    return(vec3){ p.x + q.x, p.y + q.y, p.z + q.z };
}

struct {
    // vec3 origin;
    float focalLength;
} camera;

vec2 projectToCamera(vec3 p) {
    float z = camera.focalLength / p.z;
    return (vec2){
        z * p.x + HALF_SCREEN_WIDTH,
        z * p.y + HALF_SCREEN_HEIGHT,
    };
}

typedef struct {
    int id0, id1, id2, id3;
} quad;

typedef struct {
    size_t vertBufferSize;
    vec3 *vertBuffer;
    size_t quadBufferSize;
    quad *quadBuffer;

    vec3 origin;
    float scale;
} mesh;

void FreeMesh(mesh *m) {
    free(m->vertBuffer);
    free(m->quadBuffer);
}

void GenerateCubeMesh(mesh *m) {
    m->vertBufferSize = 8;
    m->vertBuffer     = (vec3*)malloc(sizeof(vec3)*8);
    m->vertBuffer[0]  = (vec3){-1.0f, -1.0f, -1.0f};   // Bottom Back Left
    m->vertBuffer[1]  = (vec3){ 1.0f, -1.0f, -1.0f};   // Bottom Back Right
    m->vertBuffer[2]  = (vec3){ 1.0f, -1.0f,  1.0f};   // Bottom Front Right
    m->vertBuffer[3]  = (vec3){-1.0f, -1.0f,  1.0f};   // Bottom Front Left
    m->vertBuffer[4]  = (vec3){-1.0f,  1.0f, -1.0f};   // Top Back Left 
    m->vertBuffer[5]  = (vec3){ 1.0f,  1.0f, -1.0f};   // Top Back Right
    m->vertBuffer[6]  = (vec3){ 1.0f,  1.0f,  1.0f};   // Top Front Right
    m->vertBuffer[7]  = (vec3){-1.0f,  1.0f,  1.0f};   // Top Front Left

    m->quadBufferSize = 6;
    m->quadBuffer     = (quad*)malloc(sizeof(quad)*6);
    m->quadBuffer[0]  = (quad){ 0, 1, 2, 3 };          // Bottom
    m->quadBuffer[1]  = (quad){ 0, 1, 5, 4 };          // Back
    m->quadBuffer[2]  = (quad){ 4, 5, 6, 7 };          // Top
    m->quadBuffer[3]  = (quad){ 3, 2, 6, 7 };          // Front
    m->quadBuffer[4]  = (quad){ 0, 3, 7, 4 };          // Left
    m->quadBuffer[5]  = (quad){ 1, 2, 6, 5 };          // Right
}

void RenderMesh(Surface *screen, char c, mesh *m) {
    for ( int q = 0; q < m->quadBufferSize; q++ ) {
        vec2 v0 = projectToCamera( vec3Add( vec3MultiplyValue( m->vertBuffer[m->quadBuffer[q].id0], m->scale ), m->origin ) );
        vec2 v1 = projectToCamera( vec3Add( vec3MultiplyValue( m->vertBuffer[m->quadBuffer[q].id1], m->scale ), m->origin ) );
        vec2 v2 = projectToCamera( vec3Add( vec3MultiplyValue( m->vertBuffer[m->quadBuffer[q].id2], m->scale ), m->origin ) );
        vec2 v3 = projectToCamera( vec3Add( vec3MultiplyValue( m->vertBuffer[m->quadBuffer[q].id3], m->scale ), m->origin ) );

        DrawLineV(screen, c, v0, v1);
        DrawLineV(screen, c, v1, v2);
        DrawLineV(screen, c, v2, v3);
        DrawLineV(screen, c, v3, v0);

    }
}

int mainCRTStartup() {
    // Initialize screen
    Surface screen[SCREEN_SIZE] = {0};
    InitConsole();

    camera.focalLength = 64.0f;
    // camera.origin      = (vec3){0.0f, 0.0f, 0.0f};

    mesh cube;
    GenerateCubeMesh(&cube);
    cube.origin = (vec3){ 0.0f, 0.0f, 100.0f };
    cube.scale  = 10.0f;

    
    while ( true ) {
        ClearScreen(screen);

        RenderMesh(screen, '#', &cube);
        
        // vec2 q = projectToCamera( vec3Add( vec3MultiplyValue( cube.vertBuffer[4], cube.scale ), cube.origin ) );
        // wsprintf(screen+SCREEN_WIDTH+1, "%d %d", (int)q.x, (int)q.y);
        // DrawChar(screen, '@', (int)q.x, (int)q.y);

        UpdateConsole(screen); // Update screen
        Sleep(FRAMETIME);
    }
    
    FreeMesh(&cube);
    CloseConsole();
}