#include "body.hpp"

Body::Body(float rad, glm::vec3 pos, glm::vec3 vel, float mass) : m_pos(pos), m_velocity(vel), m_mass(mass) {
    m_acceleration = glm::vec3(0, 0, 0);
    m_force = glm::vec3(0, 0, 0);
    m_sphere = new Sphere(rad);
}

void Body::updateAcc() {
    glm::vec3 acc = this->m_force / this->m_mass;
    this->m_acceleration = acc;
}

void Body::updateVel() {
    // v = at
    this->m_velocity += this->m_acceleration * deltaTime;
}

void Body::updatePos() {
    this->m_pos += this->m_velocity * deltaTime;
}
