#pragma once

#include "globals.hpp"
#include "mesh.hpp"

class Sphere {
private:
    Mesh* m_mesh;

public:
    Sphere(float r);
    ~Sphere() { free(m_mesh); };

    inline Mesh* getMesh() { return m_mesh; }
    // void draw();
    // void drawInstanced(unsigned int count);
};
