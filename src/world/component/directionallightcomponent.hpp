#pragma once
#include "component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../gl/texture.hpp"

struct DirectionalLightComponent : public Component<DirectionalLightComponent>
{
	DirectionalLightComponent();

	float zNear = 1.0f;
	float zFar = 10.0f;
	float size = 10.0f;

	glm::vec3 color = { 255,0,0 };
	glm::mat4 projection = glm::ortho(-size, size, -size, size, zNear, zFar);
	std::shared_ptr<Texture> shadowMap = std::make_shared<Texture>(1024, 1024, GL_RED);

	void recalculateProjectionMatrix();

	// Inherited via Component
	virtual std::string name() override;
	virtual void registerImGui() override;
};