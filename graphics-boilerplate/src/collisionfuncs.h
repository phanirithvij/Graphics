#include "main.h"

#ifndef COLLISION_FUNCS_H
#define COLLISION_FUNCS_H

bool pointCircle(float px, float py, float cx, float cy, float r);

double distPoints(double x1, double y1, double x2, double y2);

bool linePoint(float x1, float y1, float x2, float y2, float px, float py);

bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh);

void print(float x, float y, float z, char *string);

#endif // COLLISION_FUNCS_H
