#pragma once

#include "renderpass.hpp"

#include "../../gl/mesh.hpp"

#include <glm/glm.hpp>

class LightingRenderPass : public RenderPass {
public:
	enum InputAttachment : GLint { position = 0, normal, diffuseSpecular, depth, OcclusionMap };

	LightingRenderPass();
	virtual ~LightingRenderPass();

	virtual void render(World& world);
	virtual void resize(unsigned int width, unsigned int height);

	virtual void registerImGui();
	inline virtual std::string name() { return "LightingRenderPass"; };

private:
	std::shared_ptr<Mesh> _plane;
	struct DirLight {
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	DirLight _dirLight{glm::vec3(0, -1, 0), glm::vec3(0.1, 0.1, 0.1), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5)};
};
