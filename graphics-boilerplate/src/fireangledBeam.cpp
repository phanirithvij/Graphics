#include "fireangledBeam.h"

#include "main.h"
#include "stdlib.h"
#include <iostream>

FireBeamAngled::FireBeamAngled(float x, float y, float width, float angle, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = angle;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    part_radius = 0.2;

    // me: bebop, love: I know, me: wtf is wrong with me, love: I know right
    GLfloat vertex_buffer_data[] = {
        4.0,   0.1, 0.0,
        4.0,  -0.1, 0.0, //
        -4.0, -0.1, 0.0,

        4.0,   0.1, 0.0,
        -4.0,  0.1, 0.0, // 
        -4.0, -0.1, 0.0
    };

    double scale = width / 8.0;
    int num_triangles = 2;

    this->width = 8.0f * scale;
    this->height = 0.2f * scale;

    for (int i=0; i < num_triangles * (3 * 3) ; i++){
        vertex_buffer_data[i] *= scale;
    }

    this->part1 = Coin(x, y, part_radius, angle, COLOR_BLACK);
    // this->part1 = Coin(x-(width/2.0 + part_radius), y, part_radius, angle, COLOR_BLACK);
    this->part2 = Coin(x, y, part_radius, angle, COLOR_GREEN);
    // this->part2 = Coin(x+(width/2.0 + part_radius), y, part_radius, angle, COLOR_GREEN);

    this->object = create3DObject(GL_TRIANGLES, num_triangles*3, vertex_buffer_data, color, GL_FILL);
}

void FireBeamAngled::draw(glm::mat4 VP) {
    float xoff = this->width/2.0 + part_radius;
    this->part1.draw(VP, -xoff);
    this->part2.draw(VP, xoff);

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

void FireBeamAngled::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool FireBeamAngled::onscreen(){
    // std::cout << "FireBeamAngled on screen" << std::endl;
    bool retval_ = (this->position.x < screen_center_x + 4.0) && (this->position.x > screen_center_x - 4.0)
        && (this->position.y < screen_center_y + 4.0) && (this->position.y > screen_center_y - 4.0);
    if (retval_)
    // std::cout << "Damn " << retval_ << std::endl;
    return retval_;
}

bool FireBeamAngled::visited(){
    return (this->position.x < screen_center_x - 4.0);
}

void FireBeamAngled::tick(float speed) {
    // std::cout << "ticking" << std::endl;
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

bounding_box_t FireBeamAngled::bounding_box() {
    bounding_box_t bbox;
    bbox.x = this->position.x;
    bbox.y = this->position.y;
    bbox.width = this->width;
    bbox.height = this->height;
    bbox.type = types_shapes_t::CIRLCE;
    return bbox;
}
