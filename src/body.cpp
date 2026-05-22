#include "body.hpp"
#include "physics.hpp"
#include "deltaTime.hpp"

Body::Body(float rad, glm::vec3 pos, glm::vec3 vel, float mass) : m_pos(pos),  m_mass(mass), m_velocity(vel) {
    m_acceleration = glm::vec3(0, 0, 0);
    m_force = glm::vec3(0, 0, 0);
    m_sphere = new Sphere(rad);
}

void Body::updateAcc() {
    glm::vec3 acc = this->m_force / this->m_mass;
    this->m_acceleration = acc;
}

void Body::updateVel() {
    this->m_velocity += this->m_acceleration * deltaTime;
}

void Body::updatePos() {
    this->m_pos += this->m_velocity * deltaTime;
}

void Body::calcGravForceVec(Body* other, glm::vec3& outForce1, glm::vec3& outForce2) {
    calcGravityForceVectors(this->m_mass, other->getMass(), this->m_pos, other->getPos(), outForce1, outForce2);
    // glm::vec3 dist = other->m_pos - this->m_pos;
    // float distSq = glm::dot(dist, dist);
    // float forceMagnitude = calcGravityForceMagnitude(this->m_mass, other->m_mass, distSq);
    // outForce1 = dist * forceMagnitude / this->m_mass;
    // outForce2 = dist * forceMagnitude / other->m_mass;
}
