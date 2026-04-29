#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

inline int WIN_HEIGHT = 720;
inline int WIN_WIDTH = 1280;
constexpr float cameraDefaultYaw = -90.f;
constexpr float cameraDefaultPitch = 0.f;
constexpr float cameraDefaultSensitivity = 5.f;
constexpr float cameraDefaultZoom = 30.f;
constexpr float cameraDefaultFOV = 90.f;
constexpr float cameraDefaultSpeed = 5.f;