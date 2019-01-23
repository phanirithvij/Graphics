#include "main.h"

#ifndef JETPARTICLE_H
#define JETPARTICLE_H


class Jetparticle {
public:
    Jetparticle() {}
    Jetparticle(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedX;
    double speedY;
    double width;
    double height;
    int64_t start_time;
    int64_t curr_time;
    double initPosY;
    double offsetX;
    bool canDelete();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // JETPARTICLE_H
