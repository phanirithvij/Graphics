#pragma once
#include "main.h"

// #ifndef Island_H
// #define Island_H


class Island {
public:
    Island() {}
    Island(float x, float y, float z, color_t color);
    glm::vec3 position;
    glm::vec3 rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(float offset);
    double speed;
    state_cam_t camera_state;
private:
    VAO *object;
};

// #endif // Island_H
