#pragma once

#include "globals.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

inline float calcGravityForceMagnitude(float mass1, float mass2, float dist) {
    if (dist < 0.0001f) return 0.0f;
    return G * ((mass1 * mass2) / pow(dist, 2));
}

inline void calcGravityForceVectors(float mass1, float mass2, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 &outForce1, glm::vec3 &outForce2) {
    float magnitude = calcGravityForceMagnitude(mass1, mass2, glm::distance(pos1, pos2));
    if (magnitude == 0.0f) {
        outForce1 = glm::vec3(0.f);
        outForce2 = glm::vec3(0.f);
        return;
    }

    // 1) direction btwn the 2
    glm::vec3 normalizedDirVect1 = glm::normalize(pos2 - pos1);
    glm::vec3 normalizedDirVect2 = glm::normalize(pos1 - pos2);

    // 2) force vector for body1
    outForce1 = magnitude * normalizedDirVect1;

    // 3) force vector for body2
    outForce2 = magnitude * normalizedDirVect2;
}

class Body;

void stepPhysics(std::vector<Body*>& bodies);

inline int substepsFor(float speed) {
    return std::max(1, int(std::ceil(speed)));
}

inline int clampSpeedExponent(int exponent) {
    return std::min(std::max(exponent, minSpeedExponent), maxSpeedExponent);
}
