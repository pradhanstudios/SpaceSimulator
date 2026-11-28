#pragma once

#include "globals.hpp"
#include "mesh.hpp"

std::vector<float> generateSphereVertices(float r);

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
