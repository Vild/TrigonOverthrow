#pragma once

#include "renderpass.hpp"

#include "../../gl/mesh.hpp"

#include <glm/glm.hpp>

struct DirLight {
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 direction;

	bool operator==(const DirLight& other) { return diffuse == other.diffuse && specular == other.specular && direction == other.direction; }
	bool operator!=(const DirLight& other) { return !operator==(other); }

	DirLight& operator=(const DirLight& other) {
		diffuse = other.diffuse;
		specular = other.specular;
		direction = other.direction;
		return *this;
	}
};

struct PointLight {
	glm::vec3 diffuse;
	glm::vec3 specular;

	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	bool operator==(const PointLight& other) {
		return diffuse == other.diffuse && specular == other.specular && position == other.position && constant == other.constant && linear == other.linear &&
					 quadratic == other.quadratic;
	}
	bool operator!=(const PointLight& other) { return !operator==(other); }

	PointLight& operator=(const PointLight& other) {
		diffuse = other.diffuse;
		specular = other.specular;
		position = other.position;

		constant = other.constant;
		linear = other.linear;
		quadratic = other.quadratic;

		return *this;
	}
};

class LightingRenderPass : public RenderPass {
public:
	enum InputAttachment : GLint { position = 0, normal, diffuseSpecular, depth, occlusionMap };

	LightingRenderPass();
	virtual ~LightingRenderPass();

	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;

	void registerImGui() final;
	inline std::string name() final { return "LightingRenderPass"; };

private:
	static const int MAX_POINT_LIGHTS = 16;

	std::shared_ptr<Mesh> _plane;

	bool _settings_enableDirLight = true;
	bool _settings_enablePointLight = true;

	glm::vec3 _ambient;
	DirLight _dirLight;
	PointLight _pointLights[MAX_POINT_LIGHTS];
};
