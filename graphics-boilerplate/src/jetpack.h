#include "main.h"

#ifndef JETPACK_H
#define JETPACK_H


class Jetpack {
public:
    Jetpack() {}
    Jetpack(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void moveH(int direction);
    double width;
    double height;
    double speedX;
    double speedY;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // JETPACK_H
