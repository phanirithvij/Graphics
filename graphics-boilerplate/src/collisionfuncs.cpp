#include "collisionfuncs.h"
#include "myutils.h"
#include "GL/glut.h"
#include "string.h"

//see : http://www.jeffreythompson.org/collision-detection/line-point.php`

bool pointCircle(float px, float py, float cx, float cy, float r) {
    float distX = px - cx;
    float distY = py - cy;
    float distance_square = (distX*distX) + (distY*distY);

    return (distance_square <= r * r);
}

double distPoints(double x1, double y1, double x2, double y2){
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


void print(float x, float y, float z, char *string){
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x,y);
    //get the length of the string to display
    int len = (int) strlen(string);

    //loop to display character by character
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,string[i]);
    }
};



bool linePoint(float x1, float y1, float x2, float y2, float px, float py) {

    // get distance from the point to the two ends of the line
    double d1 = distPoints(px,py, x1,y1);
    double d2 = distPoints(px,py, x2,y2);

    // get the length of the line
    double lineLen = distPoints(x1,y1, x2,y2);

    // since floats are so minutely accurate, add
    // a little buffer zone that will give collision
    double buffer = 0.1;    // higher # = less accurate

    // if the two distances are equal to the line's 
    // length, the point is on the line!
    // note we use the buffer here to give a range, 
    // rather than one #
    if (d1+d2 >= lineLen-buffer && d1+d2 <= lineLen+buffer) {
        return true;
    }
    return false;
}

bool lineLine(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

  // calculate the distance to intersection point
    float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
    float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

    // if uA and uB are between 0-1, lines are colliding
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

        // optionally, draw a circle where the lines meet
        float intersectionX = x1 + (uA * (x2-x1));
        float intersectionY = y1 + (uA * (y2-y1));
        return true;
    }
    return false;
}

// LINE/RECTANGLE
bool lineRect(float x1, float y1, float x2, float y2, float rx, float ry, float rw, float rh) {

    // check if the line has hit any of the rectangle's sides
    // uses the Line/Line function below
    bool left =   lineLine(x1, y1, x2, y2, rx,    ry,    rx,    ry+rh);
    bool right =  lineLine(x1, y1, x2, y2, rx+rw, ry,    rx+rw, ry+rh);
    bool top =    lineLine(x1, y1, x2, y2, rx,    ry,    rx+rw, ry);
    bool bottom = lineLine(x1, y1, x2, y2, rx,    ry+rh, rx+rw, ry+rh);

    // if ANY of the above are true, the line
    // has hit the rectangle
    if (left || right || top || bottom) {
        return true;
    }
    return false;
}
