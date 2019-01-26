#include "main.h"

#ifndef BALLOON_H
#define BALLOON_H


class Balloon {
public:
    Balloon() {}
    Balloon(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedX;
    double speedY;
    double gravityY;
    double width;
    double height;
    int64_t start_time;
    int64_t curr_time;
    double initPosY;
    bool onscreen();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALLOON_H
