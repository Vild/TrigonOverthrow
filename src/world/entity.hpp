#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <cstdint>

#include "../lib/sole/sole.hpp"

#include "component/component.hpp"

// Kinda like GameObject - https://docs.unity3d.com/ScriptReference/GameObject.html

class World;
class State;

class Entity {
public:
	/// Don't call this one directly! Always use world->addEntity
	Entity(sole::uuid uuid, std::string name);
	virtual ~Entity();

	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* addComponent() {
		_components.push_back(std::make_unique<T>());
		return static_cast<T*>(_components.back().get());
	}

	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* addComponent(std::unique_ptr<T> component) {
		_components.push_back(std::move(component));
		return static_cast<T*>(_components.back().get());
	}

	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* getComponent() {
		for (std::unique_ptr<Component>& c : _components) {
			T* com = dynamic_cast<T*>(c.get());
			if (com)
				return com;
		}
		return nullptr;
	}

	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	void removeComponent() {
		for (auto it = _components.begin(); it != _components.end(); it++) {
			T* com = dynamic_cast<T*>((*it).get());
			if (!com)
				continue;
			_components.erase(it);
			break;
		}
	}

	typedef void (*registerImGui_f)(Entity& self, State& state);
	registerImGui_f registerImGui = nullptr;

	inline sole::uuid& getUUID() { return _uuid; }
	inline std::string& getName() { return _name; }
	inline void makeDead() { dead = true;  }
	inline bool isDead() { return dead; }
	inline std::vector<std::unique_ptr<Component>>& getComponents() { return _components; }

private:
	sole::uuid _uuid;
	std::string _name;
	std::vector<std::unique_ptr<Component>> _components;
	
	bool dead;
};
