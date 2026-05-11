#include "renderer.hpp"
#include <iostream>

// Constructor
Renderer::Renderer() {
    m_FillDraw = true;
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glEnable(GL_DEPTH_TEST);
	std::cout << "Renderer initialized." << std::endl;
}

// Destructor
Renderer::~Renderer() {
	std::cout << "Renderer destroyed." << std::endl;
}

void Renderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Body* body, Shader* shader, Camera* camera) {
	shader->enable();
    shader->setm4("projection", glm::value_ptr(camera->getProjection()));
    shader->setm4("view", glm::value_ptr(camera->getViewForPosition(body->getPos())));
	body->getMesh()->bind();
    for (int i = 0; i < SPHERE_RESOLUTION; i++)
        glDrawArrays(GL_TRIANGLE_STRIP, static_cast<GLint>(i * (SPHERE_RESOLUTION + 1) * 2 ) , static_cast<GLsizei>(2 * (SPHERE_RESOLUTION + 1)));
    // glDrawArrays(GL_TRIANGLES, 0, body->getMesh()->getVertexCount());
	shader->disable();
}

void Renderer::setClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void Renderer::toggleWireframeDraw() {
    m_FillDraw = !m_FillDraw;
    GLenum modes[] = {GL_LINE, GL_FILL};
    glPolygonMode(GL_FRONT_AND_BACK, modes[m_FillDraw]);
}
