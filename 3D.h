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

struct {
    vec3 origin, rotation;
    float focalLength;
} camera;

vec3 CameraForward(float offset) {
    float theta = camera.rotation.y+offset;
    return (vec3){ -sinf(theta), 0.0f, cosf(theta) };
}

vec2 projectToCamera(vec3 p) {
    p = vec3Sub(p, camera.origin);
    p = vec3Rotate(p, camera.rotation.x, 0);
    p = vec3Rotate(p, camera.rotation.y, 1);
    p = vec3Rotate(p, camera.rotation.z, 2);

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
} mesh;

typedef struct {
    mesh *m;
    vec3
        origin,
        scale;
        // Rotation if I need it
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
    m->quadBuffer[3]  = (quad){ 3, 2, 6, 7 };          // Front
    m->quadBuffer[4]  = (quad){ 0, 3, 7, 4 };          // Left
    m->quadBuffer[5]  = (quad){ 1, 2, 6, 5 };          // Right
}

void GenerateQuadMesh(mesh *m) {
    m->vertBufferSize = 4;
    m->vertBuffer     = (vec3*)malloc(sizeof(vec3)*4);
    m->vertBuffer[0]  = (vec3){-1.0f,  0.0f, -1.0f};   // Back Left
    m->vertBuffer[1]  = (vec3){ 1.0f,  0.0f, -1.0f};   // Back Right
    m->vertBuffer[2]  = (vec3){ 1.0f,  0.0f,  1.0f};   // Front Right
    m->vertBuffer[3]  = (vec3){-1.0f,  0.0f,  1.0f};   // Front Left

    m->quadBufferSize = 1;
    m->quadBuffer     = (quad*)malloc(sizeof(quad));
    m->quadBuffer[0]  = (quad){ 0, 1, 2, 3 };
}

void RotateMesh(mesh *m, float theta, int axis) {
    for ( size_t v = 0; v < m->vertBufferSize; v++ )
        m->vertBuffer[v] = vec3Rotate(m->vertBuffer[v], theta, axis);
}

void RenderObject(Surface *screen, char c, obj *o) {
    for ( size_t q = 0; q < o->m->quadBufferSize; q++ ) {
        vec2 pv0 = projectToCamera( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id0], o->scale ), o->origin ) );
        vec2 pv1 = projectToCamera( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id1], o->scale ), o->origin ) );
        vec2 pv2 = projectToCamera( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id2], o->scale ), o->origin ) );
        vec2 pv3 = projectToCamera( vec3Add( vec3Multiply( o->m->vertBuffer[o->m->quadBuffer[q].id3], o->scale ), o->origin ) );

        DrawLineV(screen, c, pv0, pv1);
        DrawLineV(screen, c, pv1, pv2);
        DrawLineV(screen, c, pv2, pv3);
        DrawLineV(screen, c, pv3, pv0);
    }
}

#endif