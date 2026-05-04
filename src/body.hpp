#pragma once
#include "globals.hpp"
#include "sphere.hpp"
#include "mesh.hpp"

class Body {
public:
    Body(float rad, glm::vec3 pos);
    ~Body() { delete m_sphere; };

    inline float getRadius() { return m_sphere->getRadius(); };
    inline glm::vec3 getPos() { return m_pos; };
    inline Mesh* getMesh() { return m_sphere->getMesh(); };

    inline void setPos(glm::vec3 pos) { m_pos = pos; };
    inline void setRadius(float r) { m_sphere->setRadius(r); };

private:
    Sphere* m_sphere;
    glm::vec3 m_pos;
};
