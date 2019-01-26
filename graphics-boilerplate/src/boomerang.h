#include "main.h"

#ifndef BOOMERANG_H
#define BOOMERANG_H


class Boomerang {
public:
    Boomerang() {}
    Boomerang(float x, float y, color_t color);
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
};

bool detect_collision_player_boom(Boomerang &boome);

#endif // BOOMERANG_H
