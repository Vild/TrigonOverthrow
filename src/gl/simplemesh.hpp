#pragma once
#include "../lib/glad.h"
#include <glm/glm.hpp>
#include <vector>

class SimpleMesh
{
public:
	SimpleMesh();
	virtual ~SimpleMesh();

	SimpleMesh& setDrawMode(GLenum mode);
	SimpleMesh& addVertex(const glm::vec3 & vertex);

	void finalize();
	void draw();

private:
	GLuint VAO;
	GLuint VBO;
	GLenum drawMode;
	
	std::vector<glm::vec3> vertices;
};