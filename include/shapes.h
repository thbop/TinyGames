#ifndef SHAPES_H
#define SHAPES_H

#include "math.h"

#include "vec23.h"

#ifdef MASKED_DRAWING
void _plotLineLow(Surface *screen, char c, int x0, int y0, int x1, int y1, char mask) {
#else
void _plotLineLow(Surface *screen, char c, int x0, int y0, int x1, int y1) {
#endif
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
        #ifdef MASKED_DRAWING
        DrawMaskedChar(screen, c, x, y, mask);
        #else
        DrawChar(screen, c, x, y);
        #endif
        if ( D > 0 ) {
            y += yi;
            D += (2 * (dy - dx));
        }
        else D += 2*dy;
    }
}
#ifdef MASKED_DRAWING
void _plotLineHigh(Surface *screen, char c, int x0, int y0, int x1, int y1, char mask) {
#else
void _plotLineHigh(Surface *screen, char c, int x0, int y0, int x1, int y1) {
#endif
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
        #ifdef MASKED_DRAWING
        DrawMaskedChar(screen, c, x, y, mask);
        #else
        DrawChar(screen, c, x, y);
        #endif
        if ( D > 0 ) {
            x += xi;
            D += (2 * (dx - dy));
        }
        else D += 2*dx;
    }
}

#ifdef MASKED_DRAWING
void DrawLine(Surface *screen, char c, int x0, int y0, int x1, int y1, char mask) {
#else
void DrawLine(Surface *screen, char c, int x0, int y0, int x1, int y1) {
#endif
    if ( x0 == x1 ) {
        #ifdef MASKED_DRAWING
        if ( y1 > y0 ) for ( int y = y0; y < y1; y++ ) DrawMaskedChar(screen, c, x0, y, mask);
        else           for ( int y = y1; y < y0; y++ ) DrawMaskedChar(screen, c, x0, y, mask);
        #else
        if ( y1 > y0 ) for ( int y = y0; y < y1; y++ ) DrawChar(screen, c, x0, y);
        else           for ( int y = y1; y < y0; y++ ) DrawChar(screen, c, x0, y);
        #endif
        return;
    }
    if ( y0 == y1 ) {
        #ifdef MASKED_DRAWING
        if ( x1 > x0 ) for ( int x = x0; x < x1; x++ ) DrawMaskedChar(screen, c, x, y0, mask);
        else           for ( int x = x1; x < x0; x++ ) DrawMaskedChar(screen, c, x, y0, mask);
        #else
        if ( x1 > x0 ) for ( int x = x0; x < x1; x++ ) DrawChar(screen, c, x, y0);
        else           for ( int x = x1; x < x0; x++ ) DrawChar(screen, c, x, y0);
        #endif
        return;
    }
    if ( abs(y1 - y0) < abs(x1 - x0) ) {
        #ifdef MASKED_DRAWING
        if ( x0 > x1 ) _plotLineLow(screen, c, x1, y1, x0, y0, mask);
        else           _plotLineLow(screen, c, x0, y0, x1, y1, mask);
        #else
        if ( x0 > x1 ) _plotLineLow(screen, c, x1, y1, x0, y0);
        else           _plotLineLow(screen, c, x0, y0, x1, y1);
        #endif
    }
    else {
        #ifdef MASKED_DRAWING
        if ( y0 > y1 ) _plotLineLow(screen, c, x1, y1, x0, y0, mask);
        else           _plotLineLow(screen, c, x0, y0, x1, y1, mask);
        #else
        if ( y0 > y1 ) _plotLineLow(screen, c, x1, y1, x0, y0);
        else           _plotLineLow(screen, c, x0, y0, x1, y1);
        #endif
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

#ifdef MASKED_DRAWING
void _DrawSpan( Surface *screen, char c, TriangleSpan span, int y, char mask ) {
#else
void _DrawSpan( Surface *screen, char c, TriangleSpan span, int y ) {
#endif
    if ( span.x == span.y ) return;
    for ( int x = span.x; x < span.y; x++ )
        #ifdef MASKED_DRAWING
        DrawMaskedChar(screen, c, x, y, mask);
        #else
        DrawChar(screen, c, x, y);
        #endif
}

#ifdef MASKED_DRAWING
void _DrawSpansBetweenEdges( Surface *screen, char c, TriangleEdge edge0, TriangleEdge edge1, char mask ) {
#else
void _DrawSpansBetweenEdges( Surface *screen, char c, TriangleEdge edge0, TriangleEdge edge1 ) {
#endif
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
        TriangleSpan span = GetTriangleSpan( edge0.u.x + (int)(e0xdiff * factor0), edge1.u.x + (int)(e1xdiff * factor1) );
        #ifdef MASKED_DRAWING
        _DrawSpan(screen, c, span, y, mask);
        #else
        _DrawSpan(screen, c, span, y);
        #endif

        factor0 += factorStep0;
        factor1 += factorStep1;
    }
}
#ifdef MASKED_DRAWING
void DrawTriangle( Surface *screen, char c, vec2 u, vec2 v, vec2 w, char mask ) {
#else
void DrawTriangle( Surface *screen, char c, vec2 u, vec2 v, vec2 w ) {
#endif
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
    #ifdef MASKED_DRAWING
    _DrawSpansBetweenEdges( screen, c, edges[longEdge], edges[shortEdge0], mask );
    _DrawSpansBetweenEdges( screen, c, edges[longEdge], edges[shortEdge1], mask );
    #else
    _DrawSpansBetweenEdges( screen, c, edges[longEdge], edges[shortEdge0] );
    _DrawSpansBetweenEdges( screen, c, edges[longEdge], edges[shortEdge1] );
    #endif
}


#endif