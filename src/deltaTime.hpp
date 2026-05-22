#pragma once
#include "globals.hpp"

inline float deltaTime = 0.0f;
inline float physicsDeltaTime = 0.0f;
inline float _lastFrameTime = 0.0f;

inline void updateDeltaTime(float simSpeed = 1.f) {
    float currentFrameTime = float(glfwGetTime());
    deltaTime = currentFrameTime - _lastFrameTime;
    physicsDeltaTime = 100.f * simSpeed * deltaTime;
    _lastFrameTime = currentFrameTime;
    // std::cout << deltaTime << std::endl;
}
