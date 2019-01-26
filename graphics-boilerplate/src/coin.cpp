#include "coin.h"
#include "main.h"
#include "stdlib.h"
#include <iostream>

Coin::Coin(float x, float y, float radius, float angle, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = angle;
    this->color = color;
    this->disabled = false;
    this->score = 1;
    if (color.r == COLOR_GOLD.r && color.g == COLOR_GOLD.g && color.b == COLOR_GOLD.b){
        this->score = 5;
    }
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int n = 20;
    GLfloat vertex_buffer_data[3*3*n];
    for(int i=0;i<9*n;) {
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

bool operator<(const Coin &c1, const Coin &c2){
    return c1.position.x < c2.position.x;
}

bool operator<(const Coin &c, const float &f){
    return c.position.x < f;
}

bool operator<(const float &f, const Coin &c){
    return c.position.x > f;
}

void Coin::refresh(){
    int n = 20;
    GLfloat vertex_buffer_data[3*3*n];
    for(int i=0;i<9*n;) {
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;

        vertex_buffer_data[i++]=cos((i/9)*(((float)2*M_PI)/n)) * this->width / 2.0;
        vertex_buffer_data[i++]=sin((i/9)*(((float)2*M_PI)/n)) * this->width / 2.0;
        vertex_buffer_data[i++]=0.0;

        vertex_buffer_data[i++]=cos(((i/9)+1)*(((float)2*M_PI)/n)) * this->width / 2.0;
        vertex_buffer_data[i++]=sin(((i/9)+1)*(((float)2*M_PI)/n)) * this->width / 2.0;
        vertex_buffer_data[i++]=0.0;
    }

    this->object = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data, this->color, GL_FILL);
}

void Coin::disable(){
    this->disabled = true;
    this->color = COLOR_GREY;
    this->refresh();
}

Coin::Coin(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->color = color;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int n = 20;
    GLfloat vertex_buffer_data[(3*3)*n];
    for(int i=0;i<9*n;) {
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

void Coin::draw(glm::mat4 VP) {
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

void Coin::draw(glm::mat4 VP, float xoffset) {
    // std::cout << "Angle, XOFF " << this->rotation << " " << xoffset << std::endl;
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

void Coin::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Coin::onscreen(){
    // std::cout << "COin on screen" << std::endl;
    bool retval_ = (this->position.x < screen_center_x + 4.0) && (this->position.x > screen_center_x - 4.0)
        && (this->position.y < screen_center_y + 4.0) && (this->position.y > screen_center_y - 4.0);
    if (retval_)
    // std::cout << "Damn " << retval_ << std::endl;
    return retval_;
}

bool Coin::visited(){
    return (this->position.x < screen_center_x - 4.0);
}

void Coin::tick(float speed) {}

bounding_box_t Coin::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::CIRLCE;
    return bbox;
}

// Coin newCoinRandom(){
//     double cx, cy;
//     cx =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//     cy =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//     return Coin(cx, cy, COLOR_YELLOW);
// }
