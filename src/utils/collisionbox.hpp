#pragma once

#include <glm/glm.hpp>

struct CollisionBox2D {
	glm::vec2 position;
	glm::vec2 size;

	CollisionBox2D(glm::vec2 position, glm::vec2 size) : position(position), size(size) {
	}

	inline bool collide(const glm::vec2& point) {
		return point.x > position.x && point.x < position.x + size.x && point.y > position.y && point.y < position.y + size.y;
	}
	inline bool collide(const CollisionBox2D& other) {
		return position.x < other.position.x + other.size.x && position.x + size.x > other.position.x && position.y < other.position.y + other.size.y &&
					 position.y + size.y < other.position.y;
	}
};
