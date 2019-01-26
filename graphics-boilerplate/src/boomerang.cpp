#include "boomerang.h"
#include "main.h"
#include <iostream>
#include "player.h"
#include "math.h"
#include "myutils.h"

extern Player player;

Boomerang::Boomerang(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speedY = 0.05f;
    gravityX = 0.000008f;
    initPosX = x;
    initPosY = y;

    start_time = getEpochTime();

    speedX = 0.01f; // TODO rand speed

    GLfloat vertex_buffer_data[] = {
        -0.1, 0.1, 0,
        -0.1, -0.1, 0,
        0.1, -0.1, 0,

        -0.1, 0.1, 0,
        0.1, 0.1, 0,
        0.1, -0.1, 0,
    };
    
    int num_triangles = 2;
    this->width = 0.1;
    this->height = 0.1;

    this->object = create3DObject(GL_TRIANGLES, num_triangles*3, vertex_buffer_data, color, GL_FILL);
}

void Boomerang::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Boomerang::refresh(){
    position.x = screen_center_x + 3;
    position.y = screen_center_y + 2;
    initPosX = position.x;
    initPosY = position.y;
    start_time = getEpochTime();
}

void Boomerang::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Boomerang::onscreen(){
    bool offscreenX = (this->position.x > screen_center_x + 4.0f);
    return !offscreenX;
}

void Boomerang::tick() {
    // std::cout << "ticking" << std::endl;
    // this->rotation += speedX;
    curr_time = getEpochTime();
    this->rotation ++ ;
    this->position.y -= (/* player.speedX + */ speedY);

    int64_t t = (this->curr_time - this->start_time);

    this->position.x = this->initPosX - (speedX * t) + (0.5f * gravityX * t * t);
    // this->position.y -= speed;
}

bounding_box_t Boomerang::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::RECTANGLE;
    return bbox;
}
