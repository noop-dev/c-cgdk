#ifndef _COMMON_H_
#define _COMMON_H_

#include <math.h>
#include <stdlib.h>

typedef unsigned char ct_bool;

struct ct_point
{
    int x;
    int y;
};

/*
 * Returns new point structure
 */
static struct ct_point point(int x, int y)
{
    struct ct_point p;
    p.x = x;
    p.y = y;
    return p;
} 

/*
 * Returns square of the distance between two points as integer
 */
static int distance_squared(const struct ct_point a, const struct ct_point b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;  
}

/*
 * Returns the distance between two points as double
 */
static double distance(const struct ct_point a, const struct ct_point b) {
    return sqrt((double)distance_squared(a, b));
}

#endif
