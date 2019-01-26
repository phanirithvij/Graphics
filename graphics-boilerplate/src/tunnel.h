#include "main.h"

#ifndef TUNNEL_H
#define TUNNEL_H

class Tunnel {
public:
    Tunnel() {}
    Tunnel(float x, float y, float radius, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void draw(glm::mat4 VP, float xoffset);
    void set_position(float x, float y);
    void tick();
    double speed;
    double width;
    double height;
    bool onscreen();
    bool visited();
    void actforces();
    void disappear();
    color_t color;
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *object2;
};

bool operator<(const Tunnel &c1, const Tunnel &c2);
bool operator<(const Tunnel &c, const float &f);
bool operator<(const float &f, const Tunnel &c);

#endif // TUNNEL_H
