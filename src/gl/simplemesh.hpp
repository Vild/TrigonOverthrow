#pragma once
#include "../lib/glad.h"
#include <glm/glm.hpp>
#include <vector>
#include <initializer_list>

class SimpleMesh
{
public:
	typedef std::initializer_list<glm::vec3> vlist_t;

	SimpleMesh(GLenum drawMode, std::initializer_list<glm::vec3> vertices, GLuint maxInstances = 0);
	virtual ~SimpleMesh();

	SimpleMesh& setDrawMode(GLenum mode);

	void draw();
	void draw(std::vector<glm::mat4> & instances);

private:
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	GLenum drawMode;
	GLuint maxInstances;

	std::vector<glm::vec3> vertices;
};