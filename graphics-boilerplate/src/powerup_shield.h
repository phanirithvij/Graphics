#include "main.h"

#ifndef POWERUP_H
#define POWERUP_H


class PowerupShield {
public:
    PowerupShield() {}
    PowerupShield(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedX;
    double speedY;
    double gravityX;
    double width;
    double height;
    int64_t start_time;
    int64_t curr_time;
    double initPosY;
    double initPosX;
    void refresh();
    bool onscreen();
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *object2;
};

#endif // POWERUP_H
