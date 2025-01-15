#ifndef SHAPES_H
#define SHAPES_H

#include "math.h"

#include "vec23.h"

void _plotLineLow(Surface *screen, char c, int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if ( dy < 0 ) {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y0;

    for ( int x = x0; x < x1; x++ ) {
        DrawChar(screen, c, x, y);
        if ( D > 0 ) {
            y += yi;
            D += (2 * (dy - dx));
        }
        else D += 2*dy;
    }
}

void _plotLineHigh(Surface *screen, char c, int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if ( dx < 0 ) {
        xi = -1;
        dx = -dx;
    }
    int D = (2 * dx) - dy;
    int x = x0;

    for ( int y = y0; y < y1; y++ ) {
        DrawChar(screen, c, x, y);
        if ( D > 0 ) {
            x += xi;
            D += (2 * (dx - dy));
        }
        else D += 2*dx;
    }
}

void DrawLine(Surface *screen, char c, int x0, int y0, int x1, int y1) {
    if ( x0 == x1 ) {
        if ( y1 > y0 ) for ( int y = y0; y < y1; y++ ) DrawChar(screen, c, x0, y);
        else           for ( int y = y1; y < y0; y++ ) DrawChar(screen, c, x0, y);
        return;
    }
    if ( y0 == y1 ) {
        if ( x1 > x0 ) for ( int x = x0; x < x1; x++ ) DrawChar(screen, c, x, y0);
        else           for ( int x = x1; x < x0; x++ ) DrawChar(screen, c, x, y0);
        return;
    }
    if ( abs(y1 - y0) < abs(x1 - x0) ) {
        if ( x0 > x1 ) _plotLineLow(screen, c, x1, y1, x0, y0);
        else           _plotLineLow(screen, c, x0, y0, x1, y1);
    }
    else {
        if ( y0 > y1 ) _plotLineLow(screen, c, x1, y1, x0, y0);
        else           _plotLineLow(screen, c, x0, y0, x1, y1);
    }
}

typedef struct {
    vec2 u, v;
} TriangleEdge;

TriangleEdge GetTriangleEdge(vec2 u, vec2 v) {
    if (v.y > u.y) return (TriangleEdge){ u, v };
    else           return (TriangleEdge){ v, u };
}

#define TriangleSpan vec2

TriangleSpan GetTriangleSpan(float x0, float x1) {
    if ( x1 > x0 ) return (TriangleSpan){ x0, x1 };
    else           return (TriangleSpan){ x1, x0 };
}

void _DrawSpan( Surface *screen, char c, TriangleSpan span, int y ) {
    if ( span.x == span.y ) return;
    for ( int x = span.x; x < span.y; x++ )
        DrawChar(screen, c, x, y);
}

void _DrawSpansBetweenEdges( Surface *screen, char c, TriangleEdge edge0, TriangleEdge edge1 ) {
    float e0ydiff = edge0.v.y - edge0.u.y;
    if ( !e0ydiff ) return;

    float e1ydiff = edge1.v.y - edge1.u.y;
    if ( !e1ydiff ) return;

    float e0xdiff = edge0.v.x - edge0.u.x;
    float e1xdiff = edge1.v.x - edge1.u.x;

    float factorStep0 = 1.0f / e0ydiff;
    float factor0     = ( edge1.u.y - edge0.u.y ) * factorStep0;
    float factorStep1 = 1.0f / e1ydiff;
    float factor1     = 0.0f;

    for ( int y = edge1.u.y; y < edge1.v.y; y++ ) {
        TriangleSpan span = GetTriangleSpan( edge0.u.x + e0xdiff * factor0, edge1.u.x + e1xdiff * factor1 );
        _DrawSpan(screen, c, span, y);

        factor0 += factorStep0;
        factor1 += factorStep1;
    }
}

void DrawTriangle( Surface *screen, char c, vec2 u, vec2 v, vec2 w ) {
    TriangleEdge edges[] = {
        GetTriangleEdge(u, v),
        GetTriangleEdge(v, w),
        GetTriangleEdge(w, u),
    };

    int
        maxLength  = 0,
        longEdge   = 0,
        shortEdge0 = 0,
        shortEdge1 = 0;
    
    // Find tallest edge
    for ( int i = 0; i < 3; i++ ) {
        int length = edges[i].v.y - edges[i].u.y;
        if ( length > maxLength ) {
            maxLength = length;
            longEdge = i;
        }
    }
    shortEdge0 = ( longEdge + 1 ) % 3;
    shortEdge1 = ( longEdge + 2 ) % 3;
    _DrawSpansBetweenEdges( screen, c, edges[longEdge], edges[shortEdge0] );
    _DrawSpansBetweenEdges( screen, c, edges[longEdge], edges[shortEdge1] );
}


#endif