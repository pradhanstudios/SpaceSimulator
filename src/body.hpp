#pragma once

#include "sphere.hpp"
#include "mesh.hpp"

class Body {
public:
    Body(float rad, glm::vec3 pos, glm::vec3 vel = glm::vec3(0, 0, 0), float mass = 1.0f);
    ~Body() { delete m_sphere; };

    // getters
    inline float getRadius() { return m_sphere->getRadius(); };
    inline glm::vec3 getPos() { return m_pos; };
    inline Mesh* getMesh() { return m_sphere->getMesh(); };
    inline float getMass() { return m_mass; };
    inline glm::vec3 getVelocity() { return m_velocity; };
    inline glm::vec3 getAcceleration() { return m_acceleration; };
    inline glm::vec3 getForce() { return m_force; };
    inline glm::vec3 getMomentum() { return m_momentum; };

    // setters
    inline void setRadius(float r) { m_sphere->setRadius(r); };
    inline void setMass(float mass) { m_mass = mass; };
    inline void setPos(glm::vec3 pos) { m_pos = pos; };
    inline void setAcceleration(glm::vec3 acc) { m_acceleration = acc; };
    inline void setVelocity(glm::vec3 vel) { m_velocity = vel; };
    inline void setForce(glm::vec3 force) { m_force = force; };
    inline void setMomentum(glm::vec3 momentum) { m_momentum = momentum; };

    // physics
    void updatePos();
    void updateVel();
    void updateAcc();

    void calcGravForceVec(Body* other, glm::vec3& outForce1, glm::vec3& outForce2);

private:
    Sphere* m_sphere;
    glm::vec3 m_pos;

    // physics
    float m_mass;

    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    glm::vec3 m_force;
    glm::vec3 m_momentum;
};
