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
inline const char *vertexShaderPath = "assets/shader.vert";
inline const char *fragmentShaderPath = "assets/shader.frag";

// Window
inline int defaultHeight = 720;
inline int defaultWidth = 1280;
inline int defaultFPS = 60;

// Camera
constexpr float cameraDefaultYaw = -90.f;
constexpr float cameraDefaultPitch = 0.f;
constexpr float cameraDefaultSensitivity = 5.f;
constexpr float cameraDefaultZoom = 30.f;
constexpr float cameraDefaultFOV = 90.f;
constexpr float cameraDefaultSpeed = 5.f;
