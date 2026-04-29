#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.hpp"
#include "sphere.hpp"

int main() {
    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Space Simulator", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    Sphere test = Sphere(2, 3, 4, 5);
    std::cout << test << "\n";

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); 

        // Render








        // 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
