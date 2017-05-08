#pragma once

#include "../lib/glad.h"
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <functional>
#include "texture.hpp"

#include "shader.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 uv;
	glm::vec3 tangent;
	float lifeTime;
};

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	Mesh(const Mesh& other);
	virtual ~Mesh();

	Mesh& addBuffer(const std::string& name, std::function<void(GLuint)> bindHelper, GLenum type = GL_ARRAY_BUFFER);

	void finalize();

	void render(size_t count = 1, GLenum drawMode = GL_TRIANGLES);

	template <typename T>
	Mesh& uploadBufferData(const std::string& name, const T& data) {
		try {
			GLuint id = _extraBuffers.at(name);
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(T), glm::value_ptr(data));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		} catch (std::out_of_range e) { //-V565
		}
		return *this;
	}

	template <typename T>
	Mesh& uploadBufferArray(const std::string& name, const std::vector<T>& data) {
		try {
			GLuint id = _extraBuffers.at(name);
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(T), &data[0]);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		} catch (std::out_of_range e) { //-V565
		}
		return *this;
	}

private:
	std::vector<Vertex> _vertices;
	std::vector<GLuint> _indices;

	GLuint _vao;
	GLuint _vbo;
	GLuint _ibo;

	std::map<std::string, GLuint> _extraBuffers;

	static const int MAX_INSTANCE = 16;

	void _makeBuffers();
	void _uploadData();
};
