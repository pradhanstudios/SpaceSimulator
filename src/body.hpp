#pragma once
#include "globals.hpp"
#include "sphere.hpp"

class Body {
public:
    Body(float rad, glm::vec3 pos);
    ~Body() { delete m_sphere; };

private:
    Sphere* m_sphere;
    glm::vec3 m_pos;
};
