#include "main.h"
#include "coin.h"

#ifndef FIREBEAM_H
#define FIREBEAM_H

class FireBeam {
public:
    FireBeam() {}
    FireBeam(float x, float y, float width, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedX;
    double speedY;
    double part_radius;
    double width;
    double height;
    bool onscreen();
    bool visited();
    void moveY(int direction);
    Coin part2;
    Coin part1;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // FIREBEAM_H
