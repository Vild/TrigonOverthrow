// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "mesh.hpp"

#include "../engine.hpp"
#include <cstddef>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) : _vertices(vertices), _indices(indices) {
	_makeBuffers();
	_uploadData();
}

Mesh::~Mesh() {
	for (std::map<std::string, GLuint>::iterator it = _extraBuffers.begin(); it != _extraBuffers.end(); ++it) {
		GLuint id = it->second;
		glDeleteBuffers(1, &id);
	}

	GLuint buffers[2] = {_vbo, _ibo};
	glDeleteBuffers(sizeof(buffers) / sizeof(*buffers), buffers);

	glDeleteVertexArrays(1, &_vao);
}

Mesh& Mesh::addBuffer(const std::string& name, std::function<void(GLuint)> bindHelper, GLenum type) {
	glBindVertexArray(_vao);
	GLuint id;
	glGenBuffers(1, &id);

	_extraBuffers[name] = id;
	bindHelper(id);

	return *this;
}

void Mesh::finalize() {
	// TODO:
}

void Mesh::render(size_t count, GLenum drawMode) {
	glBindVertexArray(_vao);
	glDrawElementsInstanced(drawMode, _indices.size(), GL_UNSIGNED_INT, NULL, count);
}

void Mesh::_makeBuffers() {
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	GLuint buffers[2];
	glGenBuffers(sizeof(buffers) / sizeof(*buffers), buffers);
	_vbo = buffers[0];
	_ibo = buffers[1];
}

void Mesh::_uploadData() {
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(GLuint), _indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(ShaderAttributeID::vertPos);
	glEnableVertexAttribArray(ShaderAttributeID::vertNormal);
	glEnableVertexAttribArray(ShaderAttributeID::vertColor);
	glEnableVertexAttribArray(ShaderAttributeID::vertUV);
	glEnableVertexAttribArray(ShaderAttributeID::vertTangent);

	glVertexAttribPointer(ShaderAttributeID::vertPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glVertexAttribPointer(ShaderAttributeID::vertNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glVertexAttribPointer(ShaderAttributeID::vertColor, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glVertexAttribPointer(ShaderAttributeID::vertUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
	glVertexAttribPointer(ShaderAttributeID::vertTangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));
}
