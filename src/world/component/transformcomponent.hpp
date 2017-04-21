#pragma once
#include "component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <btBulletDynamicsCommon.h>

// Note! When making changes here add them also to FloorTransformComponent

class TransformComponent : public Component<TransformComponent> 
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	glm::mat4 getMatrix();
	glm::vec3 getPosition();
	glm::vec3 getDirection();

	void setScale(const glm::vec3 & scale);
	void setPosition(const glm::vec3 & position);
	void setDirection(const glm::vec3 & direction, const glm::vec3 & up = {0, 1, 0});

	void move(const glm::vec3 & delta);

private:
	bool dirty;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	glm::mat4 matrix;

	void recalculateMatrix();
	
	virtual void registerImGui();

	virtual std::string name() { return "TransformComponent"; }
};
