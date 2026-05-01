#include "sim.hpp"
#include <iostream>

// Constructor
Sim::Sim(int fps)
	: m_window(nullptr), m_renderer(nullptr), m_shader(nullptr), m_fps(fps) {
	init();
	std::cout << "Sim initialized." << std::endl;
}

// Destructor
Sim::~Sim() {
    if (m_shader) {
		delete m_shader;
		m_shader = nullptr;
	}
	if (m_renderer) {
		delete m_renderer;
		m_renderer = nullptr;
	}
	if (m_window) {
		delete m_window;
		m_window = nullptr;
	}
    if (m_camera) {
        delete m_camera;
        m_camera = nullptr;
    }
	std::cout << "Sim components cleaned up." << std::endl;
}

void Sim::init() {
	m_window = new Window(defaultWidth, defaultHeight, "Space Simulation");
	m_renderer = new Renderer();
	m_shader = new Shader(vertexShaderPath, fragmentShaderPath);
    m_camera = new Camera(glm::vec3(0.f, 0.f, 10.f));

	m_sphere = new Sphere(); // segfault

    glfwSetWindowUserPointer(m_window->getGLFWwindow(), this);
    glfwSetCursorPos(m_window->getGLFWwindow(), 0, 0);
    glfwSetInputMode(m_window->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window->getGLFWwindow(), mouseCallback);

	if (m_shader->getID() == 0) {
		std::cerr << "ERROR: Shader program failed to create. Exiting." << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Sim initialization complete." << std::endl;
}

// Main loop
void Sim::run() {
	std::cout << "Sim running..." << std::endl;
    deltaTime = 1 / m_fps;
    std::this_thread::sleep_for(std::chrono::milliseconds(int(deltaTime * 1000)));
	while (!m_window->shouldClose()) {
		processInput();	// User input
		update();		// Sim state update
		render();       // Draw frame

		m_window->swapBuffers();
		m_window->pollEvents();
        updateDeltaTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(std::max(int(1000 / m_fps - deltaTime * 1000), 0)));
	}
	std::cout << "Sim loop finished." << std::endl;
}

void Sim::processInput() {
	if (m_window->isKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(m_window->getGLFWwindow(), true);
	}

	if (m_window->isKeyPressed(GLFW_KEY_W)) {
	    m_camera->setPosition(m_camera->getPosition() + m_camera->getFront() * cameraDefaultSpeed * deltaTime);	
	}

	if (m_window->isKeyPressed(GLFW_KEY_S)) {
	    m_camera->setPosition(m_camera->getPosition() - m_camera->getFront() * cameraDefaultSpeed * deltaTime);	
	}

    if (m_window->isKeyPressed(GLFW_KEY_A)) {
	    m_camera->setPosition(m_camera->getPosition() - m_camera->getRightAxis() * cameraDefaultSpeed * deltaTime);
	}

    if (m_window->isKeyPressed(GLFW_KEY_D)) {
	    m_camera->setPosition(m_camera->getPosition() + m_camera->getRightAxis() * cameraDefaultSpeed * deltaTime);	
	}

    if (m_window->isKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
        m_camera->setPosition(m_camera->getPosition() - glm::vec3(0.f, 1.f, 0.f) * cameraDefaultSpeed * deltaTime);
    }

    if (m_window->isKeyPressed(GLFW_KEY_SPACE)) {
        m_camera->setPosition(m_camera->getPosition() + glm::vec3(0.f, 1.f, 0.f) * cameraDefaultSpeed * deltaTime);
    }
}

void Sim::mouseCallback(GLFWwindow* window, double posX, double posY) {
    Sim* sim = static_cast<Sim*>(glfwGetWindowUserPointer(window));

    double offsetX = (sim->m_mousePosX - posX) * deltaTime;
    double offsetY = (sim->m_mousePosY - posY) * deltaTime;
    sim->m_mousePosX = posX;
    sim->m_mousePosY = posY; 
    sim->m_camera->processMouse(offsetX, offsetY);
}

void Sim::update() {
	// Movement, physics, AI, animation, updates, etc
    m_camera->updateView();
}

void Sim::render() {
	m_renderer->clear();

	// m_renderer->draw(*m_chunk->getMesh(), *m_shader, *m_camera);
}