#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, float radius, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void draw(glm::mat4 VP, float xoffset);
    void set_position(float x, float y);
    void tick(float speed);
    double speed;
    double width;
    double height;
    bool onscreen();
    bool visited();
    color_t color;
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *object2;
};

bool operator<(const Magnet &c1, const Magnet &c2);
bool operator<(const Magnet &c, const float &f);
bool operator<(const float &f, const Magnet &c);

#endif // MAGNET_H
