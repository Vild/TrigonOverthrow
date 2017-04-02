#include "player.hpp"

#include "../component/transform.hpp"

Player::Player() {
	addComponent<TransformComponent>();
}