#include "main.h"
#include "timer.h"
#include "ball.h"
#include "player.h"
#include "platform.h"
#include "myutils.h"
#include "balloon.h"
#include "jetpack.h"
#include "jetparticles.h"
#include "coin.h"
#include "bits/stdc++.h"
#include "firebeam.h"
#include "fireangledBeam.h"
#include "magnet.h"
#include "collisionfuncs.h"
#include "boomerang.h"
#include "powerup_shield.h"
#include "powerup_life.h"
#include <chrono>

#include "GL/glut.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Player player;
Platform plat;
Jetpack jet;
Balloon showballoon;
FireBeamAngled fbangled;
Boomerang boom;
PowerupShield sheild;
PowerupLife life_up;

vector<Jetparticle> j_particles;
vector<Balloon> balloonlist;
vector<Coin> coins;
vector<FireBeam> firebeams;
vector<FireBeamAngled> fbangledvec;
vector<Magnet> magnets;
vector<PowerupLife> showlives;

int score_match = 0;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 90;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // glm::vec3 eye (player.position.x, player.position.y, 5);
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f) + player.position.x, player.position.y, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (player.position.x, player.position.y, 0);
    glm::vec3 target (0, 0, 0);

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
    plat.draw(VP);

    for (int i = j_particles.size() -1; i > -1 ; i--){
        if (j_particles[i].canDelete()){
            j_particles.erase(j_particles.begin() + i); // delete the particle
        } else {
            if(player.isFlying) j_particles[i].draw(VP);
        }
    }

    for (int i=0; i < magnets.size(); i++){
        magnets[i].draw(VP);
    }

    for (int i=0; i < firebeams.size(); i++){
        firebeams[i].draw(VP);
    }

    for (int i = 0; i < coins.size(); i++){
        if (coins[i].onscreen()){ //  && !coins[i].visited()
            // cout << "Bazinga " << i << endl;
            coins[i].draw(VP);
        }
    }

    fbangled.draw(VP);

    sheild.draw(VP);

    player.draw(VP);
    showballoon.draw(VP);
    jet.draw(VP);

    for (int i=0; i < balloonlist.size(); i++){
        (balloonlist[i].position.y > screen_center_y + 4.5f) ? balloonlist[i].position.y = screen_center_y + 4.5f : 1;

        if (balloonlist[i].position.y < -3.0f) {
            balloonlist[i].position.y = -2.96f;
        }

        if (!balloonlist[i].onscreen()){
            balloonlist.erase(balloonlist.begin() + i); // delete the balloon
        } else {
            cout << "Drawing balloon" << endl;
            balloonlist[i].draw(VP);
        }
    }

    boom.draw(VP);
    life_up.draw(VP);

    for (int i =0; i< showlives.size();i++){
        showlives[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int upkey = glfwGetKey(window, GLFW_KEY_UP);
    int balloonkey = glfwGetKey(window, GLFW_KEY_S);
    int anglekey = glfwGetKey(window, GLFW_KEY_DOWN);
    // int camerakey = glfwGetKey(window, GLFW_KEY_C);

    // if (camerakey){
    //     camera_rotation_angle ++ ;
    // }
    
    if (anglekey) {
        fbangled.rotation++;
        fbangled.part1.rotation++;
        fbangled.part2.rotation++;
    }

    if (left) {
        // cout << "Wanna move by " << (player.width + jet.width)<< endl;
        if (player.position.x > screen_center_x -  4.0 + (player.width + jet.width)){
            player.moveH(-1);
            jet.moveH(-1);
        }
    }

    if (right) {
        // if (player.position.x < screen_center_x + 4.0){
        player.moveH(1);
        // std::cout << "POS X : " << player.position.x << std::endl;
        jet.moveH(1);
        // }
        if (player.position.x > screen_center_x){
            plat.position.x += player.speedX;
            screen_center_x += player.speedX;
            // std::cout<<plat.position.x<<" "<<screen_center_x<<std::endl;
        }
    }

    if (space || upkey) {
        player.start_time = getEpochTime();
        player.floatup();
    }

    if (balloonkey) {
        Balloon balloon = Balloon(player.position.x - (player.width / 2.0), player.position.y, COLOR_SKYBLUE);
        balloon.start_time = getEpochTime();
        balloonlist.push_back(balloon);
    }
}

void createParticles(){
    if (player.isFlying){
        Jetparticle middleparticle =     Jetparticle(jet.position.x,      jet.position.y - (jet.height / 2.0f) - 0.4f, COLOR_MIDNIGHTBLUE);
        // Jetparticle particle_left_most = Jetparticle(jet.position.x-0.21f, jet.position.y - (jet.height / 2.0f) - 0.4f, COLOR_BLACK);
        Jetparticle particle_left =      Jetparticle(jet.position.x-0.11f, jet.position.y - (jet.height / 2.0f) - 0.5f, COLOR_BLACK);
        Jetparticle particle_right =      Jetparticle(jet.position.x+0.11f, jet.position.y - (jet.height / 2.0f) - 0.5f, COLOR_BLACK);
        // Jetparticle particle_right_most = Jetparticle(jet.position.x+0.21f, jet.position.y - (jet.height / 2.0f) - 0.4f, COLOR_BLACK);
        middleparticle.start_time =      getEpochTime();
        particle_left.start_time =       getEpochTime();
        particle_right.start_time =      getEpochTime();
        // particle_left_most.start_time =  getEpochTime();
        // particle_right_most.start_time = getEpochTime();
        j_particles.push_back(middleparticle);
        j_particles.push_back(particle_left);
        j_particles.push_back(particle_right);
        // j_particles.push_back(particle_left_most);
        // j_particles.push_back(particle_right_most);
    }
}

void handleLives(){
    if (player.lives < 0){
        cout << "DEAD" << endl;
        // exit(0);
    }
    showlives.clear();
    for (int i=0; i < player.lives; i++){
        PowerupLife showli = PowerupLife(screen_center_x + 3.8 - (i * 0.2) , screen_center_y + 4-0.1 , COLOR_RED);
        showlives.push_back(showli);
    }
}


void tick_elements() {
    createParticles();

    handleLives();

    jet.tick();
    player.tick();

    reset_screen();

    showballoon.position.y = player.position.y;
    showballoon.position.x = player.position.x - player.width / 2.0;

    for (int i=0; i < balloonlist.size(); i++){
        balloonlist[i].curr_time = getEpochTime();
        balloonlist[i].tick();
    }

    for (int i=0; i < j_particles.size(); i++){
        j_particles[i].curr_time = getEpochTime();
        j_particles[i].tick();
    }

    for (int i=0; i < firebeams.size(); i++){
        firebeams[i].tick();
    }

    for (int i=0; i < magnets.size(); i++){
        if (magnets[i].onscreen()){
            magnets[i].tick();
        }
        if (magnets[i].visited()){
            magnets.erase(magnets.begin() + i);
        }
    }

    boom.tick();
    sheild.tick();
    life_up.tick();

    if (!boom.onscreen()){
        cout << "Boom respawning" << endl;
        boom.refresh();
    }
    // camera_rotation_angle += 1;
    // cout << camera_rotation_angle << endl;
}


void initGL(GLFWwindow *window, int width, int height){
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    plat           = Platform(0.0f, -3.0f, COLOR_RED);
    player         = Player(-2.0f, 0.0f, COLOR_MIDNIGHTBLUE);
    jet            = Jetpack(-2.0f, 0.0f, COLOR_JETPACK);
    showballoon    = Balloon(-2.0 - (player.width / 2.0), 0.0, COLOR_SKYBLUE);
    boom           = Boomerang(4, 3, COLOR_RED2);
    sheild         = PowerupShield(-4, 0, COLOR_GREY);
    life_up        = PowerupLife(-4 , 3, COLOR_RED2);

    int num_coins = 100;

    mt19937 gen(189); // see : https://stackoverflow.com/a/29648692/8608146

    // Generate pseudo-random numbers
    // uniformly distributed in range (0, 4)
    uniform_real_distribution<> xdist(0, 4);
    uniform_real_distribution<> ydist(-2, 4);

    // Generate ten pseudo-random numbers
    for (int i = 0; i < num_coins; i++) {
        double randomX = xdist(gen) * 10;
        double randomY = ydist(gen) * 2;

        if (randomY < 4 && randomY > -4 && randomX > screen_center_x - 4){
            color_t color_coin = COLOR_YELLOW;
            if(rand() % 2 == 0){
                color_coin = COLOR_GOLD;
            }
            Coin coin = Coin(randomX, randomY, 0.2, 0, color_coin);
            coins.push_back(coin);
        }
    }

    sort(coins.begin(), coins.end());

    //set start time for the first time
    player.start_time = getEpochTime();
    
    for(int i=0; i < 10; i++){
        FireBeam firebeam = FireBeam(i * 10, ydist(gen), 3.0, COLOR_ORANGE);
        firebeams.push_back(firebeam);
    }

    for (int i=1; i < 7 ; i++){
        Magnet m_mag = Magnet(i * 10, 3 + ydist(gen), 0.5, COLOR_MAGNET);
        magnets.push_back(m_mag);
    }
    
    fbangled = FireBeamAngled(0, 0, 3.0, 45, COLOR_ORANGE);

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

bool detect_collision_waterballoon_firebeam(const Balloon &wb, const FireBeam &fb){
    bool detected = linePoint(fb.part1.position.x, fb.part1.position.y,fb.part2.position.x, fb.part2.position.y  ,wb.position.x, wb.position.y);
    return detected;
}

bool detect_collision_player_boom(Boomerang &boome){
    bool collided = detect_collision(player.bounding_box(), boome.bounding_box());
    return collided;
}

bool detect_collision_player_shield(){
    bool collided = detect_collision(player.bounding_box(), sheild.bounding_box());
    return collided;
}

bool detect_collision_player_life(){
    bool collided = detect_collision(player.bounding_box(), life_up.bounding_box());
    return collided;
}

void on_Collide_jetpack_player(){
    //restrict jetpack to move further
    jet.position.x = player.position.x - (jet.width /2.0 + player.width / 2.0);
}

void on_Collide_sheild_player(){
    //activate sheild
    sheild.activate();
}

void detect_collisions_all(){
    //jetpack and player
    if (detect_collision(jet.bounding_box(), player.bounding_box())) {
        on_Collide_jetpack_player();
    }
    //player coins
    int lb_index = lower_bound(coins.begin(), coins.end(), player.position.x - 1) - coins.begin();
    int ub_index = upper_bound(coins.begin(), coins.end(), player.position.x + 1) - coins.begin();

    for (int i = lb_index ; i < ub_index ; i++){
        if(detect_collision(coins[i].bounding_box(), player.bounding_box())){
            cout << "Score ++ bro good for you " << i << endl;
            score_match+=coins[i].score;
            cout << "Curr score " << score_match << endl;
            coins.erase(coins.begin() + i); //delete coin
        }
    }

    //water balloon firebeam or fireangledbeam
    for (int i =0; i < balloonlist.size(); i++){
        for (int j=0; j < firebeams.size(); j++){
            if (firebeams[j].onscreen() && !firebeams[j].disabled){
                if (detect_collision_waterballoon_firebeam(balloonlist[i], firebeams[j])){
                    firebeams[j].disabled = true;
                    firebeams[j].refresh();
                    balloonlist.erase(balloonlist.begin() + i);
                }
            } else if(firebeams[j].visited()) {
                firebeams.erase(firebeams.begin() + j);
            }
        }
    }

    //player and firebeam or fireangledbeam
    for (int i=0; i < firebeams.size(); i++){
        if (detect_collision(player.bounding_box(), firebeams[i].bounding_box())){
            if (!firebeams[i].disabled && !sheild.active){
                //kill health
                player.lives--;
                firebeams[i].disabled = true;
                firebeams[i].refresh();
                cout << "Lives " << player.lives << endl;
            } else if (!firebeams[i].disabled && sheild.active) {
                sheild.deactivate();
                firebeams[i].disabled = true;
                firebeams[i].refresh();
                cout << "Deactviating sheild" << endl;
            } else {

            }
        }
    }
    for (int i=0; i < fbangledvec.size(); i++){
        if (detect_collision(player.bounding_box(), fbangledvec[i].bounding_box())){
            if (!fbangledvec[i].disabled){
                //kill health
            }
        }
    }
    //player and boomerang
    if (detect_collision_player_boom(boom)){
        cout << "BOOOM" << endl;
        if (sheild.active){
            sheild.deactivate();
        } else {
            player.lives--;
        }
    }

    //player and sheild
    if (detect_collision_player_shield()) {
        if (sheild.active){
            sheild.deactivate();
        }
        on_Collide_sheild_player();
    }

    //player and life
    if (detect_collision_player_life() && !life_up.destroyed){
        life_up.activate();
        life_up.destroyed = true;
    }

}

int main(int argc, char **argv) {

    // system("aplay ./src/mario_coin_sound.mp3 &");
    system("ffplay -nodisp -autoexit src/mario_coin_sound.mp3 >/dev/null 2>&1 &");

    //init glut
    glutInit( & argc, argv );

    srand(time(0));
    int width  = 1000;
    int height = 1000;

    // int width  = 400;
    // int height = 400;
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
            detect_collisions_all();
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    if (a.type != b.type) {
        //then a circle and a rectangle
        return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
               (abs(a.y - b.y) * 2 < (a.height + b.height));
    } else { /* same types so distance btw centers */
        return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
               (abs(a.y - b.y) * 2 < (a.height + b.height));
    }
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
