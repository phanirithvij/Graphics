#include "powerup_life.h"
#include "main.h"
#include <iostream>
#include "player.h"
#include "math.h"
#include "myutils.h"

extern Player player;

PowerupLife::PowerupLife(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speedY = 0.05;
    speedX = 0.001;
    this->color = color;
    this->active = false;
    destroyed = false;
    initPosX = x;
    initPosY = y;

    GLfloat vertex_buffer_data[] = {
        0, -0.5, 0,
        0.5, 0, 0,
        -0.5, 0, 0,

        -0.5, 0, 0,
        -0.25, 0.5, 0,
        0, 0.25, 0,

        0, 0.25, 0,
        0.25, 0.5, 0,
        0.5, 0, 0,

        -0.5, 0, 0,
        0.5, 0, 0,
        0, 0.25, 0,
    };

    int num_triangles = 4;

    double scale = 0.2;

    for (int i=0; i < num_triangles * 3 * 3 ; i++){
        vertex_buffer_data[i] *= scale;
    }

    this->width = 1 * scale;
    this->height = 1 * scale;

    this->object = create3DObject(GL_TRIANGLES, (num_triangles) * 3, vertex_buffer_data, color, GL_FILL);
}

void PowerupLife::draw(glm::mat4 VP) {
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

void PowerupLife::refresh(){
    position.x = screen_center_x + 3;
    position.y = screen_center_y + 2;
    initPosX = position.x;
    initPosY = position.y;
    start_time = getEpochTime();
}

void PowerupLife::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool PowerupLife::onscreen(){
    bool offscreenX = (this->position.x > screen_center_x + 4.0f);
    return !offscreenX;
}

void PowerupLife::activate(){
    player.lives++;
    GLfloat vertbuff[] = {};
    this->object = create3DObject(GL_TRIANGLES, 0, vertbuff, color, GL_FILL);    
}

void PowerupLife::deactivate(){
    active = false;
    GLfloat vertbuff[] = {};
    this->object = create3DObject(GL_TRIANGLES, 0, vertbuff, color, GL_FILL);    
}


void PowerupLife::tick() {
    // std::cout << "ticking" << std::endl;
    // this->rotation += speedX;
    curr_time = getEpochTime();
    // rotation ++ ;

    int64_t t = (curr_time - start_time);

    position.x = initPosX + speedX * t;
    position.y = initPosY + 3 * sin(position.x * 5);
    // position.y -= speed;
}

bounding_box_t PowerupLife::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::CIRLCE;
    return bbox;
}
