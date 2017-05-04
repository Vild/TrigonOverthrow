#pragma once
#include "../lib/glad.h"
#include <glm/glm.hpp>
#include <vector>

class SimpleMesh {
public:
	SimpleMesh();
	virtual ~SimpleMesh();

	SimpleMesh& setDrawMode(GLenum mode);
	SimpleMesh& addVertex(const glm::vec3& vertex);

	void finalize(int maxInstances = 0);
	void draw();
	void draw(std::vector<glm::mat4>& instances);

private:
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	GLenum drawMode;
	GLuint maxInstances;

	std::vector<glm::vec3> vertices;
};