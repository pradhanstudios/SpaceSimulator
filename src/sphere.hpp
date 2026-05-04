#pragma once

#include "globals.hpp"
#include "mesh.hpp"

class Sphere {
private:
    Mesh* m_mesh;
    float m_radius;

public:
    Sphere(float r);
    ~Sphere() { delete m_mesh; };

    inline Mesh* getMesh() { return m_mesh; }
    inline float getRadius() { return m_radius; }

    void setRadius(float r);
    // void draw();
    // void drawInstanced(unsigned int count);
};
