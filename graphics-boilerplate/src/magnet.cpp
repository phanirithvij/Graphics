#include "magnet.h"
#include "main.h"
#include "stdlib.h"
#include <iostream>

Magnet::Magnet(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->color = color;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int n = 20;
    GLfloat vertex_buffer_data[3*3*n];
    for(int i=0;i<(9*n / 2);) {
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

    this->width = 2.0f * radius;
    this->height = 2.0f * radius;

    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}


void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::draw(glm::mat4 VP, float xoffset) {
    std::cout << "Angle, XOFF " << this->rotation << " " << xoffset << std::endl; 
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    rotate          = rotate * glm::translate(glm::vec3(xoffset, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Magnet::onscreen(){
    // std::cout << "Magnet on screen" << std::endl;
    bool retval_ = (this->position.x < screen_center_x + 4.0) && (this->position.x > screen_center_x - 4.0)
        && (this->position.y < screen_center_y + 4.0) && (this->position.y > screen_center_y - 4.0);
    if (retval_)
    // std::cout << "Damn " << retval_ << std::endl;
    return retval_;
}

bool Magnet::visited(){
    return (this->position.x < screen_center_x - 4.0);
}

void Magnet::tick(float speed) {
    // std::cout << "ticking" << std::endl;
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Magnet::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::CIRLCE;
    return bbox;
}

// Magnet newMagnetRandom(){
//     double cx, cy;
//     cx =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//     cy =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//     return Magnet(cx, cy, COLOR_YELLOW);
// }