// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "entity.hpp"

Entity::Entity(sole::uuid uuid, std::string name) : _uuid(uuid), _name(name) {}

Entity::~Entity() {}
