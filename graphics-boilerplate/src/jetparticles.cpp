#include "jetparticles.h"
#include "main.h"
#include <iostream>
#include "jetpack.h"
#include "player.h"
#include "math.h"
// #include "myutils.h"

extern Player player;
extern Jetpack jet;

Jetparticle::Jetparticle(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    initPosY = y;
    offsetX = - jet.position.x + x;

    int n = 12;
    speedY = -0.001f;
    speedX = (offsetX > 0) ? 0.0002f : -0.0002f;
    if (offsetX == 0){speedX = 0;}

    GLfloat vertex_buffer_data[9*n];
    for(int i=0;i<9*n;) {
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;
        vertex_buffer_data[i++]=0.0;
        
        vertex_buffer_data[i++]=cos((i/9)*(((float)2*M_PI)/n)) * 0.02f;
        vertex_buffer_data[i++]=sin((i/9)*(((float)2*M_PI)/n)) * 0.02f;
        vertex_buffer_data[i++]=0.0;
        
        vertex_buffer_data[i++]=cos(((i/9)+1)*(((float)2*M_PI)/n)) * 0.02f;
        vertex_buffer_data[i++]=sin(((i/9)+1)*(((float)2*M_PI)/n)) * 0.02f;
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

void Jetparticle::draw(glm::mat4 VP) {
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

void Jetparticle::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Jetparticle::canDelete(){
    bool dist_gt_3f = (this->position.y + 0.3f < this->initPosY);
    return dist_gt_3f;
}

void Jetparticle::tick() {
    // std::cout << "ticking" << std::endl;
    // this->rotation += speedX;
    int64_t t = (this->curr_time - this->start_time);

    this->position.x = jet.position.x + offsetX + speedX * t;

    this->position.y = this->initPosY + speedY * t;
    // this->position.y = jet.position.y + (speedY * t) * 0.3f;
    // this->position.y -= speed;
}

bounding_box_t Jetparticle::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::CIRLCE;
    return bbox;
}
