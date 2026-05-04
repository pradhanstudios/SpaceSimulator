#pragma once

#include "globals.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "sphere.hpp"
#include "body.hpp"
#include <GL/glew.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	void clear();

	void draw(Body* body, Shader* shader, Camera* camera);
	void setClearColor(float r, float g, float b, float a);
    void toggleWireframeDraw();

private:
    bool m_FillDraw;
};