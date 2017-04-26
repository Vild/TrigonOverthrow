#pragma once
#include "component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <btBulletDynamicsCommon.h>

// Note! When making changes here add them also to FloorTransformComponent

class TransformComponent : public Component {
public:
	TransformComponent();
	virtual ~TransformComponent();

	inline glm::mat4 getMatrix() {
		if (dirty)
			recalculateMatrix();
		return matrix;
	}
	inline glm::vec3 getPosition() { return position; }

	inline glm::quat getRotation() { return rotation; }
	inline glm::vec3 getDirection() {
		static const glm::vec3 forward = {0, 0, -1};
		return glm::mat3_cast(rotation) * forward;
	}

	inline glm::vec3 getScale() { return scale; }

	void setScale(const glm::vec3& scale);
	void setPosition(const glm::vec3& position);
	void setRotation(const glm::quat& rotation);
	void setDirection(const glm::vec3& direction, const glm::vec3& up = {0, 1, 0});

	void move(const glm::vec3& delta);

	virtual void registerImGui();
	inline std::string name() { return "TransformComponent"; }

private:
	bool dirty;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	glm::mat4 matrix;

	void recalculateMatrix();
};

inline glm::vec3 cast(btVector3 const& v) {
	return {v.x(), v.y(), v.z()};
}

inline glm::quat cast(btQuaternion const& q) {
	return {q.w(), q.x(), q.y(), q.z()};
}

inline btVector3 cast(glm::vec3 const& v) {
	return {v.x, v.y, v.z};
}

inline btQuaternion cast(glm::quat const& q) {
	return {q.x, q.y, q.z, q.w};
}
