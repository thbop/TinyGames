#ifndef LINE_H
#define LINE_H

#include "math.h"

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


#endif