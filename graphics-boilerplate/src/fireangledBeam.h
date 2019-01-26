#include "main.h"
#include "coin.h"

#ifndef FIREBEAM_ANGLED_H
#define FIREBEAM_ANGLED_H

class FireBeamAngled {
public:
    FireBeamAngled() {}
    FireBeamAngled(float x, float y, float width, float angle, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick(float speed);
    // double speed;
    double part_radius;
    double width;
    double height;
    bool onscreen();
    bool visited();
    bool disabled;
    color_t color;
    Coin part2;
    Coin part1;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // FIREBEAM_ANGLED_H
