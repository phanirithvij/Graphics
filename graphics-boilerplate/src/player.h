#include "main.h"

#ifndef PLAYER_H
#define PLAYER_H


class Player {
public:
    Player() {}
    Player(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    void floatup();
    void actforces(double start_position);
    void moveH(int direction);
    double speedX;
    double speedY;
    double gravityY;
    double width;
    double height;
    bool isFlying;
    int64_t start_time;
    int64_t curr_time;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // PLAYER_H
