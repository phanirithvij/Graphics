#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include "glm/glm.hpp"

class coord_t {
public:
    float x, y, z;
    coord_t(){};
    coord_t(float a, float b, float c):x(a), y(b), z(c){};
};

class Facec_t {
public:
    int facenum;
    bool foursides;
    int faces[4];
    Facec_t(){};
    Facec_t(int facen, int f1, int f2, int f3): facenum(facen){
        faces[0] = f1;
        faces[1] = f2;
        faces[2] = f3;
        foursides = false;
    };
    Facec_t(int facen, int f1, int f2, int f3, int f4): facenum(facen){
        faces[0] = f1;
        faces[1] = f2;
        faces[2] = f3;
        faces[3] = f4;
        foursides = true;
    };
};

int loadObject(std::string filename){

};

bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals
);
