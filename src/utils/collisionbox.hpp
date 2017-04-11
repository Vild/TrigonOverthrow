#pragma once

#include <glm/glm.hpp>

class CollisionBox2D {
public:
	inline bool collide(const glm::vec2& point) { return point.x > _pos.x && point.x < _pos.x + _size.x && point.y > _pos.y && point.y < _pos.y + _size.y; }
	inline bool collide(const CollisionBox2D& other) {
		return _pos.x < other._pos.x + other._size.x && _pos.x + _size.x > other._pos.x && _pos.y < other._pos.y + other._size.y && _pos.y + _size.y < other._pos.y;
	}

private:
	glm::vec2 _pos;
	glm::vec2 _size;
};