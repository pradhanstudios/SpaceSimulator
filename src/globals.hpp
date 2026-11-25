#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <thread>

// Shader
inline const char* vertexShaderPath = "assets/shader.vert";
inline const char* fragmentShaderPath = "assets/shader.frag";
inline const char* uiVertexShaderPath = "assets/ui.vert";
inline const char* uiFragmentShaderPath = "assets/ui.frag";
inline const char* uiFontPath = "assets/fonts/DejaVuSans.ttf";

// Window
inline int defaultHeight = 720;
inline int defaultWidth = 1280;
inline int defaultFPS = 60;
constexpr int SPHERE_RESOLUTION = 64;
inline glm::vec3 backgroundColor = {0.f, 0.f, 0.f};

// Camera
constexpr float cameraDefaultYaw = -90.f;
constexpr float cameraDefaultPitch = 0.f;
constexpr float cameraDefaultSensitivity = 5.f;
constexpr float cameraDefaultZoom = 30.f;
constexpr float cameraDefaultFOV = 75.f;
constexpr float cameraDefaultSpeed = 5.f;

// Physics
// inline const float G = 6.674 * pow(10, -11);
inline const float G = 1.f; // Makes gravity stronger just for testing purposes

// Simulation speed = 10^exponent, clamped to [0.01x, 1000x]
constexpr int minSpeedExponent = -2;
constexpr int maxSpeedExponent = 3;
