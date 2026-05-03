#include "body.hpp"

Body::Body(float rad, glm::vec3 pos) : m_pos(pos) {
    m_sphere = new Sphere(rad);
}

