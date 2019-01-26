#include "main.h"

#ifndef POWERUP_LIFE_H
#define POWERUP_LIFE_H


class PowerupLife {
public:
    PowerupLife() {}
    PowerupLife(float x, float y, color_t color);
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
    void activate();
    bool active;
    void deactivate();
    color_t color;
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *object2;
};

#endif // POWERUP_LIFE_H
