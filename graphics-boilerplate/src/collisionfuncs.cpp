#include "collisionfuncs.h"
#include "myutils.h"
// #include ""

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


void print(int x, int y,int z, char *string){
    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x,y);
    //get the length of the string to display
    // int len = (int) strlen(string);

    //loop to display character by character
    for (int i = 0; i < 1; i++) {
        // glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str ing[i]);
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
