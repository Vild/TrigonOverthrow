#include "particlesystem.hpp"

#include "../component/particlecomponent.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "../../engine.hpp"

ParticleSystem::ParticleSystem() {
	_programs.resize(2);
	_programs[0] = std::make_shared<ShaderProgram>();
	_programs[0]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_init.comp", ShaderType::compute))
		.finalize();
	_programs[0]->bind().addUniform("delta").addUniform("emitterPos").addUniform("emitterDir");

	_programs[1] = std::make_shared<ShaderProgram>();
	_programs[1]->bind().attach(std::make_shared<ShaderUnit>("assets/shaders/particles_explosion.comp", ShaderType::compute))
		.finalize();
	_programs[1]->bind().addUniform("delta")
		.addUniform("swap");

	_particleData = std::make_shared<GBuffer>();
	_particleData->bind().attachTexture(Attachment::inPosition, 512, 512, GL_RGBA32F, GL_FLOAT, 4) // Input pos and life
		.attachTexture(Attachment::inVelocity, 512, 512, GL_RGBA32F, GL_FLOAT, 4)  // Input vel
		.attachTexture(Attachment::outPosition, 512, 512, GL_RGBA32F, GL_FLOAT, 4)  // Output pos and life.
		.attachTexture(Attachment::outVelocity, 512, 512, GL_RGBA32F, GL_FLOAT, 4); // Output vel
	 // TO-DO: Pong pos and vel framebuffers between computations for maximum efficineny.
}

//#pragma omp parallel for schedule(dynamic, 128)
void ParticleSystem::update(World& world, float delta) {
	int work_grp_size[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);

	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
		work_grp_size[0], work_grp_size[1], work_grp_size[2]);

	int work_grp_cnt[3];

	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

	printf("max global (total) work group size x:%i y:%i z:%i\n",
		work_grp_size[0], work_grp_size[1], work_grp_size[2]);
	for (std::shared_ptr<ParticleComponent> comp : ParticleComponent::getActiveComponents()) {
		auto * p = &comp->particles[0];
		int count = comp->_nrOfParticles;
		if (comp->init) {
			_programs[0]->bind().setUniform("delta", delta)
				.setUniform("emitterPos", comp->emitter->pos)
				.setUniform("emitterDir", comp->emitter->direction)
				.setUniform("init", comp->swap);
			comp->swap = false;
			_particleData->bindImageTexture(0, true);
			_particleData->bindImageTexture(1, true);
			// Barrier is in particlerenderpass.
			glDispatchCompute((GLuint)512, (GLuint)512, 1);
			//comp->init = false;
		}
		//else {
		//	_programs[1]->bind().setUniform("delta", delta)
		//		.setUniform("swap", comp->swap);
		//	if (comp->swap) {
		//		_particleData->bindImageTexture(0, true)
		//			.bindImageTexture(1, true)
		//			.bindImageTexture(2, false)
		//			.bindImageTexture(3, false);
		//		glDispatchCompute((GLuint)32, (GLuint)32, 1);
		//		comp->swap = false;
		//	}
		//	else {
		//		_particleData->bindImageTexture(0, false)
		//			.bindImageTexture(1, false)
		//			.bindImageTexture(2, true)
		//			.bindImageTexture(3, true);
		//		glDispatchCompute((GLuint)32, (GLuint)32, 1);
		//		comp->swap = true;
		//	}
		//	//for (int i = 0; i < count; i++) {
		//	//	p[i].lifeSpan -= delta * 1.0f;
		//	//	if (p[i].lifeSpan <= 0) {
		//	//		p[i].lifeSpan = ((rand()*1.0) / (float)INT_MAX) * 4 + 1;
		//	//		p[i].velocity = glm::normalize(glm::vec3(sin(3 * i), cos(i * 0.5f), cos(i * 0.5f) + sin(8 * i) + sin(3 * i)));
		//	//	}
		//	//}
		//}
	}
}

std::shared_ptr<GBuffer> ParticleSystem::getGBuffers() {
	return _particleData;
}