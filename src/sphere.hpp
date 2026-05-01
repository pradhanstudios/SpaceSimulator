#pragma once

#include "globals.hpp"
#include "mesh.hpp"

class Sphere {
private:
    unsigned int m_lats;
    unsigned int m_longs;
    Mesh* m_mesh;

public:
    Sphere(float r = 1.f, unsigned int lats = 64, unsigned int longs = 64);
    ~Sphere() { free(m_mesh); };

    void Draw();
    void DrawInstanced(unsigned int count);
};
