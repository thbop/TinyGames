#ifndef a3D_H
#define a3D_H


#define PI 3.141592f

typedef struct {
    float x, y;
} vec2;

#define VEC2ZERO (vec2){0.0f, 0.0f}

#define DrawLineV(screen, c, l0, l1) \
    DrawLine(screen, c, (int)l0.x, (int)l0.y, (int)l1.x, (int)l1.y )

typedef struct {
    float x, y, z;
} vec3;

#define VEC3ZERO (vec3){0.0f, 0.0f, 0.0f}

vec3 vec3Add(vec3 p, vec3 q) {
    return (vec3){ p.x + q.x, p.y + q.y, p.z + q.z };
}

vec3 vec3Sub(vec3 p, vec3 q) {
    return (vec3){ p.x - q.x, p.y - q.y, p.z - q.z };
}

// vec3 vec3MultiplyValue(vec3 p, float v) {
//     return (vec3){ p.x * v, p.y * v, p.z * v };
// }

vec3 vec3Multiply(vec3 p, vec3 q) {
    return (vec3){ p.x * q.x, p.y * q.y, p.z * q.z };
}

vec3 vec3Rotate(vec3 p, float theta, int axis) {
    if ( theta == 0.0f ) return p;
    float
        sin_theta = sinf(theta),
        cos_theta = cosf(theta);
    switch (axis) {
        case 0: return (vec3){ p.x, p.y*cos_theta-p.z*sin_theta, p.y*sin_theta+p.z*cos_theta };      break;
        case 1: return (vec3){ p.x*cos_theta + p.z*sin_theta, p.y, -p.x*sin_theta + p.z*cos_theta }; break;
        case 2: return (vec3){ p.x*cos_theta - p.y*sin_theta, p.x*sin_theta + p.y*cos_theta, p.z };  break;
    }
}

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

    m->quadBufferSize = 4;                             // actually 6 but we can cheat since we're dealing with wireframes
    m->quadBuffer     = (quad*)malloc(sizeof(quad)*4);
    m->quadBuffer[0]  = (quad){ 0, 1, 2, 3 };          // Bottom
    m->quadBuffer[1]  = (quad){ 0, 1, 5, 4 };          // Back
    m->quadBuffer[2]  = (quad){ 4, 5, 6, 7 };          // Top
    m->quadBuffer[3]  = (quad){ 3, 2, 6, 7 };          // Front
    // m->quadBuffer[4]  = (quad){ 0, 3, 7, 4 };          // Left
    // m->quadBuffer[5]  = (quad){ 1, 2, 6, 5 };          // Right
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


void RenderObject(Surface *screen, char c, obj *o) {
    for ( size_t q = 0; q < o->m->quadBufferSize; q++ ) {
        // Camera space quad
        vec3 cs0 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id0], o->scale ), o->origin ) );
        vec3 cs1 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id1], o->scale ), o->origin ) );
        vec3 cs2 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id2], o->scale ), o->origin ) );
        vec3 cs3 = toCameraSpace( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id3], o->scale ), o->origin ) );

        // If at least one point is in the view frustum
        if ( inViewFrustum(cs0) || inViewFrustum(cs1) || inViewFrustum(cs2) || inViewFrustum(cs3) ) {
            // Project draw points
            vec2 dp0 = projectToScreen(cs0);
            vec2 dp1 = projectToScreen(cs1);
            vec2 dp2 = projectToScreen(cs2);
            vec2 dp3 = projectToScreen(cs3);

            // Draw quad
            DrawLineV(screen, c, dp0, dp1);
            DrawLineV(screen, c, dp1, dp2);
            DrawLineV(screen, c, dp2, dp3);
            DrawLineV(screen, c, dp3, dp0);
        }
    }
}

#endif