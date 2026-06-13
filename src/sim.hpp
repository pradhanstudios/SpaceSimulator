#pragma once

#include "mesh.hpp"
#include "body.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "deltaTime.hpp"
#include "sphere.hpp"
#include "physics.hpp"

class Sim {
public:
	// Constructor: Initializes the Sim components.
	Sim(int m_fps=defaultFPS);

	// Destructor: Cleans up Sim components.
	~Sim();

	// Runs the main Sim loop.
	void run();

private:
	Window* m_window;   // Pointer to the Window object
	Renderer* m_renderer; // Pointer to the Renderer object
	Shader* m_shader;   // Pointer to the Shader object
    Camera* m_camera;
    double m_mousePosX;
    double m_mousePosY;
    uint m_fps;
    int m_cursorMode;
    bool m_firstMouseMove;

	// temp
	std::vector<Body*> m_bodyList;

    static void voidMouseCallback(GLFWwindow* window, double posX, double posY) {}

	// Private helper function to initialize all components
	void init();

	// Private helper function to handle input
	void processInput();

    static void mouseCallback(GLFWwindow* window, double posX, double posY);

	// Private helper function to update Sim state (empty for now)
	void update();

	// Private helper function to render the scene
	void render();

    inline void tab_out() {
        if (m_cursorMode == GLFW_CURSOR_DISABLED) {
            m_cursorMode = GLFW_CURSOR_NORMAL;
            glfwSetCursorPosCallback(m_window->getGLFWwindow(), voidMouseCallback);
        }

        else {
            m_cursorMode = GLFW_CURSOR_DISABLED;
            m_firstMouseMove = true;
            glfwSetCursorPosCallback(m_window->getGLFWwindow(), mouseCallback);
        }

        glfwSetInputMode(m_window->getGLFWwindow(), GLFW_CURSOR, m_cursorMode);
    }
};
