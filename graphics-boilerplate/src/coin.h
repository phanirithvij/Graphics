#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y, float radius, float angle, color_t color);
    Coin(float x, float y, float radius, color_t color);
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
    void refresh();
    void disable();
    bool disabled;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

bool operator<(const Coin &c1, const Coin &c2);
bool operator<(const Coin &c, const float &f);
bool operator<(const float &f, const Coin &c);

Coin newCoinRandom();

#endif // COIN_H
