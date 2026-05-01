#pragma once

#include "mesh.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"
#include "camera.hpp"
#include "deltaTime.hpp"

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
	// Mesh* m_cubeMesh; // Pointer to the Mesh object for the triangle
    Camera* m_camera;
    double m_mousePosX;
    double m_mousePosY;
    uint m_fps;

	// Private helper function to initialize all components
	void init();

	// Private helper function to handle input
	void processInput();

    static void mouseCallback(GLFWwindow* window, double posX, double posY);

	// Private helper function to update Sim state (empty for now)
	void update();

	// Private helper function to render the scene
	void render();

};