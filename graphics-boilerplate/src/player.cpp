#include "player.h"
#include "main.h"
#include "myutils.h"
#include <iostream>

extern float screen_center_x;
extern float screen_center_y;

Player::Player(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speedX = 0.2f;
    speedY = 0.1f;
    gravityY = 0.0000009f;
    lives = 7;
    onTunnel = false;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    // static const GLfloat vertex_buffer_data[] = {
    //     -0.5f, -0.5f, 0.0f,// triangle 1 : begin
    //     0.5f, -0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f,// triangle 1 : end
    //     -0.5f, -0.5f, 0.0f,// triangle 2 : begin
    //     -0.5f, 0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f// triangle 2 : end
    // };
    GLfloat vertex_buffer_data[] = {
        -(1/3.0), -0.4f, 0.0f,
        (1/3.0),  -0.4f, 0.0f, // triangle 1 body down
        (1/3.0),   0.4f, 0.0f,

        -(1/3.0), -0.4f, 0.0f,
        -(1/3.0),  0.4f, 0.0f, // triangle 2 body up
        (1/3.0),   0.4f, 0.0f,

        -0.5f,    0.4f, 0.0f,
        0.5f,    0.4f, 0.0f, // triangle 3 face down
        0.5f,    1.4f, 0.0f,

        -0.5f,    0.4f, 0.0f,
        -0.5f,    1.4f, 0.0f, // triangle 4 face up
        0.5f,    1.4f, 0.0f,

        -0.25f,   -0.8f, 0.0f,  
        -(1/12.0),-0.8f, 0.0f, // triangle 5 left leg down
        -(1/12.0),-0.4f, 0.0f,

        -0.25f,   -0.8f, 0.0f,  
        -0.25f,   -0.4f, 0.0f, // triangle 6 left leg up
        -(1/12.0),-0.4f, 0.0f,

        (1/12.0), -0.8f, 0.0f,  
        0.25f,    -0.8f, 0.0f, // triangle 7 right leg down
        0.25f,    -0.4f, 0.0f,

        (1/12.0), -0.8f, 0.0f,  
        (1/12.0), -0.4f, 0.0f, // triangle 8 right leg up
        0.25f,    -0.4f, 0.0f,

        -0.5f,    -0.1f, 0.0f,
        -(1/3.0), -0.1f, 0.0f, // triangle 9 hand down
        -(1/3.0),  0.1f, 0.0f,

        -0.5f,    -0.1f, 0.0f,
        -0.5f,     0.1f, 0.0f, // triangle 10 hand up
        -(1/3.0),  0.1f, 0.0f,

    };

    int num_triangles = 10;

    double scale = 0.6F;

    for (int i=0; i < num_triangles * 3 * 3; i++){
        vertex_buffer_data[i] *= scale;
    }

    width = 1.0f * scale;
    height = 2.0f * scale; //2.2 actually

    this->object = create3DObject(GL_TRIANGLES, num_triangles*3, vertex_buffer_data, color, GL_FILL);
}

void Player::draw(glm::mat4 VP) {
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

void Player::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Player::floatup(){
    this->isFlying = true;
    this->position.y += speedY;
    if (this->position.y + 1.4f * 0.6f > screen_center_y + 4.6F) this->position.y = screen_center_y + 4.6F - 1.4f * 0.6f;
}

void Player::actforces(double start_postion){

    double accln = this->gravityY;

    int64_t timenow = /* (int64_t) */(this->curr_time - this->start_time);

    this->position.y = start_postion - 0.5f * accln * timenow * timenow;

    // if (this->position.y + 1.4f * 0.6f > 4.0F) this->position.y = 4.0f - 1.4f * 0.6f;

    if (this->position.y < screen_center_y + -4.0f + 1.48f){
        /* on ground */
        this->isFlying = false;
        this->position.y = screen_center_y + -4.0f + 1.48f;
    }

    if (this->position.x + this->width / 2.0f > 0.0f) {
        /* right logic */
    }
    // std::cout << "cur" << this->curr_time << std::endl;

}

void Player::moveH(int direction){
    // if (direction == 1 && this->position.x < (1.0 - this->width/ 2.0f - 0.1f))
    this->position.x += direction * speedX;

    // if (direction == -1) 
    // this->position.x += direction * speedX;
}

void Player::tick() {
    this->curr_time = getEpochTime();
    this->actforces(this->position.y);
}

bounding_box_t Player::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::RECTANGLE; // type is to know circle or rectangle
    return bbox;
}