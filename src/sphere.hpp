#pragma once

#include "constants.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/**
 * Variables
 * 
 * position
 * size (radius)
 * 
 * opt: color
 */

class Sphere {
private:
    glm::vec3 pos;
    double r; // radius

public:
    Sphere(glm::vec3 pos, double r);
    Sphere(double x, double y, double z, double r);


    friend std::ostream& operator<<(std::ostream& os, const Sphere& s) {
        os << "Pos: (" << s.pos.x << ", " << s.pos.y << ", " << s.pos.z << "); Size: " << s.r;
        return os;
    }

};
