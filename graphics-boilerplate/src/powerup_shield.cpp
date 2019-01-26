#include "powerup_shield.h"
#include "main.h"
#include <iostream>
#include "player.h"
#include "math.h"
#include "myutils.h"

extern Player player;

PowerupShield::PowerupShield(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speedY = 0.05f;
    int n = 60;

    float radius = 1;
    GLfloat vertex_buffer_data[3*3*n];
    GLfloat vertex_buffer_data2[3*3*n];
    for(int i=0;i<(9*(n));) {
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;

        vertex_buffer_data[i++]=cos((i/9)*(((float)2*M_PI)/n)) * radius;
        vertex_buffer_data[i++]=sin((i/9)*(((float)2*M_PI)/n)) * radius;
        vertex_buffer_data[i++]=0.0;

        vertex_buffer_data[i++]=cos(((i/9)+1)*(((float)2*M_PI)/n)) * radius;
        vertex_buffer_data[i++]=sin(((i/9)+1)*(((float)2*M_PI)/n)) * radius;
        vertex_buffer_data[i++]=0.0;
    }

    for(int i=0;i<(9*(n));) {
        vertex_buffer_data2[i++]=0.0;
        vertex_buffer_data2[i++]=0.0;
        vertex_buffer_data2[i++]=0.0;

        vertex_buffer_data2[i++]=cos((i/9)*(((float)2*M_PI)/n)) * (radius - 0.3);
        vertex_buffer_data2[i++]=sin((i/9)*(((float)2*M_PI)/n)) * (radius - 0.3);
        vertex_buffer_data2[i++]=0.0;

        vertex_buffer_data2[i++]=cos(((i/9)+1)*(((float)2*M_PI)/n)) * (radius - 0.3);
        vertex_buffer_data2[i++]=sin(((i/9)+1)*(((float)2*M_PI)/n)) * (radius - 0.3);
        vertex_buffer_data2[i++]=0.0;
    }


    this->width = 2.0f * radius;
    this->height = 2.0f * radius;

    this->object = create3DObject(GL_TRIANGLES, (n) * 3, vertex_buffer_data, color, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, (n) * 3, vertex_buffer_data2, COLOR_BACKGROUND, GL_FILL);

    
    int num_triangles = 2;
    this->width = 0.1;
    this->height = 0.1;

    this->object = create3DObject(GL_TRIANGLES, num_triangles*3, vertex_buffer_data, color, GL_FILL);
}

void PowerupShield::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object2);
}

void PowerupShield::refresh(){
    position.x = screen_center_x + 3;
    position.y = screen_center_y + 2;
    initPosX = position.x;
    initPosY = position.y;
    start_time = getEpochTime();
}

void PowerupShield::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool PowerupShield::onscreen(){
    bool offscreenX = (this->position.x > screen_center_x + 4.0f);
    return !offscreenX;
}

void PowerupShield::tick() {
    // std::cout << "ticking" << std::endl;
    // this->rotation += speedX;
    curr_time = getEpochTime();
    this->rotation ++ ;
    this->position.y -= (/* player.speedX + */ speedY);

    int64_t t = (this->curr_time - this->start_time);

    this->position.x = this->initPosX - (speedX * t) + (0.5f * gravityX * t * t);
    // this->position.y -= speed;
}

bounding_box_t PowerupShield::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::RECTANGLE;
    return bbox;
}