#include "coin.h"
#include "main.h"
#include "stdlib.h"
#include <iostream>

Coin::Coin(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int n = 20;
    GLfloat vertex_buffer_data[3*3*n];
    for(int i=0;i<9*n;) {
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;
        
        vertex_buffer_data[i++]=cos((i/9)*(((float)2*M_PI)/n)) * 0.2f;
        vertex_buffer_data[i++]=sin((i/9)*(((float)2*M_PI)/n)) * 0.2f;
        vertex_buffer_data[i++]=0.0;
        
        vertex_buffer_data[i++]=cos(((i/9)+1)*(((float)2*M_PI)/n)) * 0.2f;
        vertex_buffer_data[i++]=sin(((i/9)+1)*(((float)2*M_PI)/n)) * 0.2f;
        vertex_buffer_data[i++]=0.0;

    }

    GLfloat x_ = vertex_buffer_data[3];
    GLfloat y_ = vertex_buffer_data[4];

    double sqradius = (x_ * x_ + y_ * y_);

    double radius = sqrt(sqradius);

    this->width = 2.0f * radius;
    this->height = 2.0f * radius;

    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, color, GL_FILL);
}

void Coin::draw(glm::mat4 VP) {
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

void Coin::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Coin::tick(float speed) {
    // std::cout << "ticking" << std::endl;
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t Coin::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = 1.0f;
    bbox.height = 1.0f;
    return bbox;
}

Coin newCoinRandom(){
    double cx, cy;
    cx =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    cy =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    return Coin(cx, cy, COLOR_RED);
}