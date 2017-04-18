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

class Entity {
public:
	/// Don't call this one directly! Always use world->addEntity
	Entity(sole::uuid uuid, std::string name);

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	T* addComponent() {
		T::getActiveComponents().push_back(std::make_unique<T>());
		auto* c = T::getActiveComponents().back().get();
		_components.push_back(c);
		return c;
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	T* addComponent(std::unique_ptr<T> component) {
		T::getActiveComponents().push_back(std::move(component));

		auto* c = T::getActiveComponents().back().get();
		_components.push_back(c);
		return c;
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	T* getComponent() {
		for (IComponent* c : _components) {
			T* com = dynamic_cast<T*>(c);
			if (com)
				return com;
		}
		return nullptr;
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	void removeComponent() {
		auto list = T::getActiveComponents();
		for (auto it = list.begin(); it != list.end(); it++) {
			auto com = std::dynamic_pointer_cast<T>(*it);
			if (!com)
				continue;
			list.erase(it);
			break;
		}

		for (auto it = _components.begin(); it != _components.end(); it++) {
			auto com = dynamic_cast<T*>(*it);
			if (!com)
				continue;
			_components.erase(it);
			break;
		}
	}

	void (*registerImGui)(Entity* entity) = nullptr;

	inline sole::uuid& getUUID() { return _uuid; }
	inline std::string& getName() { return _name; }
	inline std::vector<IComponent*>& getComponents() { return _components; }

private:
	sole::uuid _uuid;
	std::string _name;
	std::vector<IComponent*> _components;
};
