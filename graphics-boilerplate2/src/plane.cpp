#include "plane.h"
#include "main.h"
// #include "objloader.hpp"
#include "common/objloader.hpp"
#include <vector>

Plane::Plane(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation.x = 0;
    this->rotation.y = -90;
    this->rotation.z = 0;
    camera_state = VIEW_BACK;
    speed = 1;

    //objloader bro
    int num_triangles = 2;

    GLfloat vertex_buffer_data[] = {}; //load the obj file of the planev1.obj
    // GLfloat colorbuff[] = {};
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    // bool res = loadOBJ("assets/planev1.obj", vertices, uvs, normals);
    bool res = loadOBJ("assets/latest.obj", vertices, uvs, normals);

    std::cout << "UV SIZE " << uvs.size() << " NUMVERTCES " << vertices.size() << std::endl;
    std::cout << "RES BOOL " << res << std::endl;

    this->object = create3DObject(
        GL_TRIANGLES,
        vertices.size(), uvs.size(),
        vertices, uvs,
        color, GL_FILL);
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate             *= glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    rotate             *= glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of Plane arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plane::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}


void Plane::tick(float offset) {
    this->rotation.x += (speed * offset);
    // this->position.x -= speed;
    // this->position.y -= speed;
}

