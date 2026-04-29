#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "constants.hpp"
#include "sphere.hpp"
#include "camera.hpp"

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    Sphere test = Sphere(2, 3, 4, 5);
    Camera camera = Camera(glm::vec3(0, 0, 0));

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); 
        std::cout << camera.getPitch() << " " << camera.getYaw() << std::endl;

        // exit window on ESC
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        // Render








        //

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
