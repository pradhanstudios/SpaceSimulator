#include "mesh.hpp"
#include <iostream>

// Constructor
Mesh::Mesh(const float* vertices, size_t numVertices, const uint* indices, size_t numIndices){
	init(vertices, numVertices, indices, numIndices);
}

// Destructor
Mesh::~Mesh() {
	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
	}
    
	if (m_VBO != 0) {
		glDeleteBuffers(1, &m_VBO);
	}

    if (m_EBO != 0) {
        glDeleteBuffers(1, &m_EBO);
    }

	std::cout << "Mesh VAO, VBO, (if present) EBO deleted." << std::endl;
}

void Mesh::init(const float* vertices, size_t numVertices, const uint* indices, size_t numIndices) {
	m_VAO = 0;
	m_VBO = 0;
	m_EBO = 0;
	m_vertexCount = numVertices / 11;
	m_indexCount = numIndices;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
    if (indices != nullptr) {
        glGenBuffers(1, &m_EBO);
	}

	glBindVertexArray(m_VAO);

	// vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float), vertices, GL_STATIC_DRAW);

	// index data
    if (indices != nullptr) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint), indices, GL_STATIC_DRAW);
    }

	setupVertexAttributes();

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::cout << "Mesh created with VAO: " << m_VAO << ", VBO: " << m_VBO << ", EBO: " << m_EBO << std::endl;
}

void Mesh::bind() const {
	glBindVertexArray(m_VAO);
}

void Mesh::unbind() const {
	glBindVertexArray(0);
}

void Mesh::setupVertexAttributes() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float))); // normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(6 * sizeof(float))); // texcoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(8 * sizeof(float))); // tangent
	glEnableVertexAttribArray(3);
}
