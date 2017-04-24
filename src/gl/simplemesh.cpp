#include "simplemesh.hpp"

SimpleMesh::SimpleMesh()
{
	this->drawMode = GL_POINTS;

	glGenBuffers(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
}

SimpleMesh::~SimpleMesh()
{
}

SimpleMesh & SimpleMesh::setDrawMode(GLenum mode)
{
	drawMode = mode;
	return *this;
}

SimpleMesh & SimpleMesh::addVertex(const glm::vec3 & vertex)
{
	vertices.push_back(vertex);
	return *this;
}

void SimpleMesh::finalize()
{
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
}

void SimpleMesh::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(drawMode, 0, vertices.size());
}
