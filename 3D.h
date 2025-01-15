#ifndef a3D_H
#define a3D_H


#define PI 3.141592f


#define DrawLineV(screen, c, l0, l1) \
    DrawLine(screen, c, (int)l0.x, (int)l0.y, (int)l1.x, (int)l1.y )


#define CAMERA_CLIP_NEAR 0.0f
#define CAMERA_CLIP_FAR  1000.0f

struct {
    vec3 origin, rotation;
    float focalLength;
} camera;

vec3 CameraForward(float offset) {
    float theta = camera.rotation.y+offset;
    return (vec3){ -sinf(theta), 0.0f, cosf(theta) };
}

vec3 toCameraSpace(vec3 p) {
    p =    vec3Sub(p, camera.origin);
    p =    vec3Rotate(p, camera.rotation.x, 0);
    p =    vec3Rotate(p, camera.rotation.y, 1);
    return vec3Rotate(p, camera.rotation.z, 2);
}

unsigned char inViewFrustum(vec3 p) {
    if (
        p.z < CAMERA_CLIP_NEAR                              ||
        p.z > CAMERA_CLIP_FAR                               ||
        p.z < fabsf(camera.focalLength*p.x) / SCREEN_WIDTH  ||
        p.z < fabsf(camera.focalLength*p.y) / SCREEN_HEIGHT
    ) return false;
    return true;
}

vec2 projectToScreen(vec3 p) {
    float z = camera.focalLength / p.z;
    vec2 projected = (vec2){
        z * p.x + HALF_SCREEN_WIDTH,
        z * p.y + HALF_SCREEN_HEIGHT,
    };
    return projected;
}

typedef struct {
    int id0, id1, id2, id3;
} quad;

typedef struct {
    size_t vertBufferSize;
    vec3 *vertBuffer;
    size_t quadBufferSize;
    quad *quadBuffer;
} mesh;

typedef struct {
    mesh *m;
    vec3
        origin,
        scale;
        // rotation;
} obj;

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
    m->quadBuffer[3]  = (quad){ 7, 6, 2, 3 };          // Front
    m->quadBuffer[4]  = (quad){ 4, 7, 3, 0 };          // Left
    m->quadBuffer[5]  = (quad){ 1, 2, 6, 5 };          // Right
}

// void GenerateQuadMesh(mesh *m) {
//     m->vertBufferSize = 4;
//     m->vertBuffer     = (vec3*)malloc(sizeof(vec3)*4);
//     m->vertBuffer[0]  = (vec3){-1.0f,  0.0f, -1.0f};   // Back Left
//     m->vertBuffer[1]  = (vec3){ 1.0f,  0.0f, -1.0f};   // Back Right
//     m->vertBuffer[2]  = (vec3){ 1.0f,  0.0f,  1.0f};   // Front Right
//     m->vertBuffer[3]  = (vec3){-1.0f,  0.0f,  1.0f};   // Front Left

//     m->quadBufferSize = 1;
//     m->quadBuffer     = (quad*)malloc(sizeof(quad));
//     m->quadBuffer[0]  = (quad){ 0, 1, 2, 3 };
// }

// void GeneratePyramidMesh(mesh *m) {
//     m->vertBufferSize = 5;
//     m->vertBuffer     = (vec3*)malloc(sizeof(vec3)*5);
//     m->vertBuffer[0]  = (vec3){-1.0f, -1.0f, -1.0f};   // Back Left
//     m->vertBuffer[1]  = (vec3){ 1.0f, -1.0f, -1.0f};   // Back Right
//     m->vertBuffer[2]  = (vec3){ 1.0f, -1.0f,  1.0f};   // Front Right
//     m->vertBuffer[3]  = (vec3){-1.0f, -1.0f,  1.0f};   // Front Left
//     m->vertBuffer[4]  = (vec3){ 0.0f,  1.0f,  0.0f};   // Top

//     m->quadBufferSize = 3;
//     m->quadBuffer     = (quad*)malloc(sizeof(quad)*3);
//     m->quadBuffer[0]  = (quad){ 4, 3, 0, 1 };
//     m->quadBuffer[1]  = (quad){ 4, 2, 1, 0 };
//     m->quadBuffer[1]  = (quad){ 0, 1, 2, 3 };
// }

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
    m->quadBuffer[0]  = (quad){ 0, 3, 4, 5 };
    m->quadBuffer[1]  = (quad){ 0, 1, 2, 3 };

}


void RenderObject(Surface *screen, obj *o) {
    for ( size_t q = 0; q < o->m->quadBufferSize; q++ ) {
        // Camera space quad
        vec3 cs0 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id0], o->scale ), o->origin ) );
        vec3 cs1 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id1], o->scale ), o->origin ) );
        vec3 cs2 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id2], o->scale ), o->origin ) );
        vec3 cs3 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id3], o->scale ), o->origin ) );

        vec3 n   = vec3Cross( vec3Sub(cs0, cs1), vec3Sub(cs0, cs3) );

        // If at least one point is in the view frustum
        if (
            ( inViewFrustum(cs0) || inViewFrustum(cs1) || inViewFrustum(cs2) || inViewFrustum(cs3) ) &&
            n.z > 0.0f // dot(n, vec3(0,0,1))
        ) {
            // Project draw points
            vec2 dp0 = projectToScreen(cs0);
            vec2 dp1 = projectToScreen(cs1);
            vec2 dp2 = projectToScreen(cs2);
            vec2 dp3 = projectToScreen(cs3);

            // Draw quad
            DrawTriangle(screen, '.', dp0, dp1, dp2);
            DrawTriangle(screen, '.', dp2, dp3, dp0);

            DrawLineV(screen, '#', dp0, dp1);
            DrawLineV(screen, '#', dp1, dp2);
            DrawLineV(screen, '#', dp2, dp3);
            DrawLineV(screen, '#', dp3, dp0);

        }
    }
}

#endif