#include "body.hpp"

Body::Body(float rad, glm::vec3 pos, float mass) : m_pos(pos), m_mass(mass) {
    m_sphere = new Sphere(rad);
}

