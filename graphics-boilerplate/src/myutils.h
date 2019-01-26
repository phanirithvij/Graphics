#include <chrono>

#ifndef PHANI_MYUTILS_H
#define PHANI_MYUTILS_H

int64_t getEpochTime();

bool pnpoly(int nvert, float *vertx, float *verty, float testx, float testy);

struct lineParams {
    float a,b,c;
};

lineParams getLineEq(float x1, float y1, float x2, float y2);
float perpDist(lineParams line, float x, float y);
bool circleRectCollision(
    float rcir,
    float rrect,
    float centerX, float centerY,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    float x4, float y4
);

bool firebeamCollides();

struct fbeam {
    float c1x, c1y;
    float c2x, c2y;
    float radius;
};

double fRand(double fMin, double fMax);

#endif // PH_MYUTILS_H
