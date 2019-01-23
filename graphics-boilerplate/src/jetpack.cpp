#include "jetpack.h"
#include "main.h"
#include <iostream>
#include "player.h"
#include "math.h"
#include "myutils.h"

extern Player player;

Jetpack::Jetpack(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    static GLfloat vertex_buffer_data[] = {
        0.5,   0.5, 0.0,
        0.5,  -1.0, 0.0,  // triangle 1 : body
        -0.5, -1.0, 0.0,

        -0.5, -1.0, 0.0,
        -0.5,  0.5, 0.0, // triangle 2 : body
        0.5,   0.5, 0.0,

        -0.5, -1.4, 0.0,
        0.5,  -1.4, 0.0, // triangle 3 : tail (outlet)
        0.0,  -0.6, 0.0
    };

    double scale = 0.5;
    int num_triangles = 3;

    for (int i=0; i < num_triangles * 3 * 3; i++){
        vertex_buffer_data[i] *= scale;
    }

    this->width = 1.5f * scale;
    this->height = 1.5f * scale;
    speedX = player.speedX;
    speedY = player.speedY;

    this->object = create3DObject(GL_TRIANGLES, num_triangles*3, vertex_buffer_data, color, GL_FILL);
}

void Jetpack::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Jetpack::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Jetpack::moveH(int direction){
    this->position.x += direction * speedX;
};

void Jetpack::tick() {
    // this->position.x = player.position.x - 0.75f;
    this->position.y = player.position.y + this->height / 2.0;
}

bounding_box_t Jetpack::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::RECTANGLE;
    return bbox;
}
