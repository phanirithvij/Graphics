#include "main.h"
#include "timer.h"
#include "cube.h"
#include "plane.h"
#include "island.h"
#include "missile.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

// Cube cube1;
// Cube cube2;
Plane plane;
std::vector<Island> islands;
std::vector<Missile> missiles;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

float eyeX = 5*cos(camera_rotation_angle*M_PI/180.0f);
float eyeY = 0;
float eyeZ = 5*sin(camera_rotation_angle*M_PI/180.0f);

float targetX = 0;
float targetY = 0;
float targetZ = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( eyeX, eyeY, eyeZ );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (targetX,targetY,targetZ);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    // cube1.draw(VP);
    // cube2.draw(VP);
    plane.draw(VP);

    for (int i=0; i< islands.size(); i++){
        islands[i].draw(VP);
    }
    for (int i=0; i< missiles.size(); i++){
        missiles[i].draw(VP);
    }
}

void update_eye_ffp(){
    eyeX = plane.position.x - 10;
    eyeY = plane.position.y - 10;
    eyeZ = plane.position.z - 10;
    targetX = plane.position.x;
    targetY = plane.position.y;
    targetZ = plane.position.z;
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int W = glfwGetKey(window, GLFW_KEY_W);
    int S = glfwGetKey(window, GLFW_KEY_S);
    if (left) {
        // Do something
        plane.rotation.y --;
    } 
    if (right) {
        plane.rotation.y ++;
        // plane.rotation.y ++;
    }
    if (up) {
        plane.position.z += 0.2;
        if (plane.camera_state == VIEW_FFP)
        update_eye_ffp();
    } else if (down){
        plane.position.z -= 0.2;
        if (plane.camera_state == VIEW_FFP)
        update_eye_ffp();
     } else if (W) {
        plane.position.y += 0.2;
        if (plane.camera_state == VIEW_FFP)
        update_eye_ffp();
    } else if (S){
        plane.position.y -= 0.2;
        if (plane.camera_state == VIEW_FFP)
        update_eye_ffp();
    }
}

void tick_elements() {
    // plane.tick(1);
    // cube2.tick(0.5);
    // camera_rotation_angle += 1;
        for (int i=0; i< missiles.size(); i++){
            missiles[i].tick(1);
        }

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // cube1       = Cube(0, 0, 0, COLOR_RED);
    // cube2       = Cube(-5, 0, 0, COLOR_GREEN);
    plane          = Plane(-5, 0, 0, COLOR_GREEN);
    Island temp_is = Island(-20, -10, 0, COLOR_GREEN);

    islands.push_back(temp_is);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
