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
	struct DirLight {
		glm::vec3 diffuse;
		glm::vec3 specular;

		glm::vec3 direction;
	};

	struct PointLight {
		glm::vec3 diffuse;
		glm::vec3 specular;

		glm::vec3 position;
		float constant;
		float linear;
		float quadratic;
	};

	static const int POINT_LIGHTS = 64;

	std::shared_ptr<Mesh> _plane;

	glm::vec3 _ambient;
	DirLight _dirLight;
	PointLight _pointLights[POINT_LIGHTS];
};
