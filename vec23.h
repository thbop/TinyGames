#ifndef VEC23_H
#define VEC23_H

#include <math.h>


typedef struct {
    float x, y;
} vec2;

#define VEC2ZERO (vec2){0.0f, 0.0f}


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

float vec3Dot(vec3 p, vec3 q) {
    return p.x * q.x + p.y * q.y + p.z + q.z;
}

vec3 vec3Cross(vec3 p, vec3 q) {
    return (vec3){
        p.y*q.z - p.z*q.y,
        p.z*q.x - p.x*q.z,
        p.x*q.y - p.y*q.x,
    };
}

float vec3LengthSquared( vec3 p ) {
    return p.x*p.x + p.y*p.y + p.z*p.z;
}

#endif