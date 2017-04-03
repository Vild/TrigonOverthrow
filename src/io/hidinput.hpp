#pragma once

#include <glm/glm.hpp>

class HIDInput {
public:
	void update();

	inline const glm::ivec2& getXYDiff() { return _xyDiff; }
	inline const glm::vec3& getDirection() { return _direction; }

private:
	glm::ivec2 _xyDiff{0, 0};
	glm::vec3 _direction{0, 0, 0};
};
