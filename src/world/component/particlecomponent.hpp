#pragma once

#include "component.hpp"

#include "../../lib/glad.h"
#include "glm/glm.hpp"
#include "../src/gl/mesh.hpp"

struct ParticleComponent : public Component<ParticleComponent> {

	struct Particle {
		glm::vec3	pos = glm::vec3(0);
		glm::vec3	velocity = glm::vec3(0);
		float		lifeSpan = 0.0f;
		glm::mat4	model= glm::mat4(1);
	};

	struct Emitter
	{
		Emitter(glm::vec3 dir) { direction = dir; };
		glm::vec3 pos = glm::vec3(0);
		glm::vec3 direction;

	};
	// Should have a number (1 probably) of emitters to spew out particles from.
	// fix particle render system and a fixed size of particles for each system.

	std::shared_ptr<Emitter> emitter;
	std::vector<Particle> particles;
	std::vector<glm::mat4> matrices;
	std::shared_ptr<Mesh> _quad;

	int _nrOfParticles;
	float particleSize;

	void addEmitter(glm::vec3 dir, int nrOfParticles) { 
		emitter = std::make_shared<Emitter>(dir); 
		_nrOfParticles = nrOfParticles;
		matrices.resize(nrOfParticles);
		particleSize = 0.4f;
		particles.resize(nrOfParticles);
		std::vector<Vertex> _vertices = {
			Vertex{ glm::vec3(-0.2, 0.2, 0),	glm::vec3(0,0, -1), glm::vec3(0.3, 0.1, 0.6),	glm::vec2(0, 1) }, // Pos, normal, color, uv.
			Vertex{ glm::vec3(0.2, 0.2, 0),		glm::vec3(0,0, -1),	glm::vec3(0.6, 0.2, 0.3),	glm::vec2(1, 1) },
			Vertex{ glm::vec3(0.2, -0.2, 0),	glm::vec3(0,0, -1),	glm::vec3(0.8, 0.5, 0.3),	glm::vec2(1, 0) },
			Vertex{ glm::vec3(-0.2, -0.2, 0),	glm::vec3(0,0, -1),	glm::vec3(0.1, 0.1, 0.1),	glm::vec2(0, 0) },
		};
		std::vector<GLuint> _indices = { 0, 2, 1, 2, 0, 3 };

		_quad = std::make_shared<Mesh>(_vertices, _indices);

		_quad->addBuffer("m",
			[&](GLuint id) {
			glBindBuffer(GL_ARRAY_BUFFER, id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * nrOfParticles, NULL, GL_STATIC_DRAW); // Will only be uploaded once

			for (int i = 0; i < 4; i++) {
				glEnableVertexAttribArray(ShaderAttributeID::m + i);
				glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
				glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		})
			.finalize();
	};

	virtual std::string name() { return "ParticleComponent"; }

};
