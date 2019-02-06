#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"
#include "plane.h"
#include "island.h"
#include "missile.h"

bool   cannon_keyboard_input = true;
bool   drag_pan = false, old_cki;
double drag_oldx = -1, drag_oldy = -1;

extern float eyeX;
extern float eyeY;
extern float eyeZ;

extern float targetX;
extern float targetY;
extern float targetZ;

extern Plane plane;
extern std::vector<Missile> missiles;

extern float camera_rotation_angle;

using namespace std;

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_T:
                // rectangle_rot_status = !rectangle_rot_status;
                plane.camera_state = VIEW_TOWER;
                eyeX = 5;
                eyeY = 5;
                eyeZ = 0;
                targetX = 0;
                targetY = 0;
                targetZ = 0;

                break;

            case GLFW_KEY_M:{
                // new missile
                Missile temp_is = Missile(-0, -10, 0, COLOR_GREEN);
                missiles.push_back(temp_is);
                break;
            }
            case GLFW_KEY_C:
                // rectangle_rot_status = !rectangle_rot_status;
                plane.camera_state = VIEW_TOWER;
                eyeX = -10;
                eyeY = 5;
                eyeZ = 0;
                targetX = -5;
                targetY = 0;
                targetZ = 0;

                break;
            case GLFW_KEY_P:{
                // triangle_rot_status = !triangle_rot_status;
                plane.camera_state = VIEW_FFP;
                eyeX = plane.position.x - 10;
                eyeY = plane.position.y - 10;
                eyeZ = plane.position.z - 10;
                targetX = plane.position.x;
                targetY = plane.position.y;
                targetZ = plane.position.z;
                break;
            }
            case GLFW_KEY_X:
                // do something ..
                break;
            default:
                break;
        }
    } else if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            quit(window);
            break;
        default:
            break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar(GLFWwindow *window, unsigned int key) {
    switch (key) {
    case 'Q':
    case 'q':
        quit(window);
        break;
    default:
        break;
    }
}

/* Executed when a mouse button is pressed/released */
void mouseButton(GLFWwindow *window, int button, int action, int mods) {
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        if (action == GLFW_PRESS) {
            // Do something
            return;
        } else if (action == GLFW_RELEASE) {
            // Do something
        }
        break;
    // case GLFW_MOUSE_BUTTON_RIGHT:
    // if (action == GLFW_RELEASE) {
    // rectangle_rot_dir *= -1;
    // }
    // break;
    default:
        break;
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // Do something
}
