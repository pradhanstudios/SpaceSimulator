#pragma once

#include "globals.hpp"

inline float calcGravityForceMagnitude(float mass1, float mass2, float distSqd) {
    if (distSqd < 0.0001f) return 0.0f;
    return G * ((mass1 * mass2) / distSqd);
}

inline void calcGravityForceVectors(float mass1, float mass2, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 &outForce1, glm::vec3 &outForce2) {
    float magnitude = calcGravityForceMagnitude(mass1, mass2, glm::distance(pos1, pos2));

    // 1) direction btwn the 2
    glm::vec3 normalizedDirVect1 = glm::normalize(pos2 - pos1);
    glm::vec3 normalizedDirVect2 = glm::normalize(pos1 - pos2);

    // 2) force vector for body1
    outForce1 = magnitude * normalizedDirVect1;

    // 3) force vector for body2
    outForce2 = magnitude * normalizedDirVect2;
}

// inline void calcGravityForceVectors(Body* b1, Body* b2, glm::vec3& outForce1, glm::vec3& outForce2) {
//     float mass1 = b1->getMass();
//     float mass2 = b2->getMass();
//     glm::vec3 pos1 = b1->getPos();
    // glm::vec3 pos2 = b2->getPos();

//     float magnitude = calcGravityForceMagnitude(mass1, mass2, glm::distance(pos1, pos2));

//     // 1) direction btwn the 2
//     glm::vec3 normalizedDirVect1 = glm::normalize(pos2 - pos1);
//     glm::vec3 normalizedDirVect2 = glm::normalize(pos1 - pos2);

//     // 2) force vector for body1
//     outForce1 = magnitude * normalizedDirVect1;

//     // 3) force vector for body2
//     outForce2 = magnitude * normalizedDirVect2;
// }
