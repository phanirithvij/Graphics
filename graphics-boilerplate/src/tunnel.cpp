#include "tunnel.h"
#include "player.h"
#include "jetpack.h"
#include "main.h"
#include "stdlib.h"
#include <iostream>
#include "collisionfuncs.h"

extern Player player;
extern Jetpack jet;

Tunnel::Tunnel(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = -90;
    this->color = color;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int n = 60;
    GLfloat vertex_buffer_data[3*3*n];
    GLfloat vertex_buffer_data2[3*3*n];
    for(int i=0;i<(9*(n / 2));) {
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

    for(int i=0;i<(9*(n / 2));) {
        vertex_buffer_data2[i++]=0.0;
        vertex_buffer_data2[i++]=0.0;
        vertex_buffer_data2[i++]=0.0;

        vertex_buffer_data2[i++]=cos((i/9)*(((float)2*M_PI)/n)) * (radius - 0.1);
        vertex_buffer_data2[i++]=sin((i/9)*(((float)2*M_PI)/n)) * (radius - 0.1);
        vertex_buffer_data2[i++]=0.0;

        vertex_buffer_data2[i++]=cos(((i/9)+1)*(((float)2*M_PI)/n)) * (radius - 0.1);
        vertex_buffer_data2[i++]=sin(((i/9)+1)*(((float)2*M_PI)/n)) * (radius - 0.1);
        vertex_buffer_data2[i++]=0.0;
    }


    this->width = 2.0f * radius;
    this->height = 2.0f * radius;

    this->object = create3DObject(GL_TRIANGLES, (n/2) * 3, vertex_buffer_data, color, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, (n/2) * 3, vertex_buffer_data2, COLOR_BACKGROUND, GL_FILL);
}

void Tunnel::actforces(){
    double dist = distPoints(player.position.x, player.position.y, position.x, position.y);
    if (dist < 4 && dist > 0.4){
        player.onTunnel = true;
    }
}

void Tunnel::draw(glm::mat4 VP) {
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

void Tunnel::draw(glm::mat4 VP, float xoffset) {
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

void Tunnel::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Tunnel::onscreen(){
    // std::cout << "Tunnel on screen" << std::endl;
    bool retval_ = (this->position.x < screen_center_x + 4.0) && (this->position.x > screen_center_x - 4.0)
        && (this->position.y < screen_center_y + 4.0) && (this->position.y > screen_center_y - 4.0);
    if (retval_)
    // std::cout << "Damn " << retval_ << std::endl;
    return retval_;
}

bool Tunnel::visited(){
    return (this->position.x < screen_center_x - 4.0);
}

void Tunnel::tick() {
    actforces();
}

bounding_box_t Tunnel::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::CIRLCE;
    return bbox;
}

// Tunnel newTunnelRandom(){
//     double cx, cy;
//     cx =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//     cy =  static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
//     return Tunnel(cx, cy, COLOR_YELLOW);
// }