#pragma once

#include "globals.hpp"
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
    Camera* m_camera;
    double m_mousePosX;
    double m_mousePosY;
    int m_fps;

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