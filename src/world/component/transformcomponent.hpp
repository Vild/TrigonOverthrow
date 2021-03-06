#pragma once
#include "component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <btBulletDynamicsCommon.h>

class TransformComponent : public Component {
public:
	TransformComponent();
	TransformComponent(const ComponentValues& value);
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

	void registerImGui() final;
	inline std::string name() final { return "TransformComponent"; }

private:
	bool dirty;

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;
	glm::mat4 matrix;

	void recalculateMatrix();
};

inline glm::vec3 cast(btVector3 const& v) {
	return glm::vec3{v.x(), v.y(), v.z()};
}

inline glm::quat cast(btQuaternion const& q) {
	return glm::quat{q.w(), q.x(), q.y(), q.z()};
}

inline btVector3 cast(glm::vec3 const& v) {
	return btVector3{v.x, v.y, v.z};
}

inline btQuaternion cast(glm::quat const& q) {
	return btQuaternion{q.x, q.y, q.z, q.w};
}
