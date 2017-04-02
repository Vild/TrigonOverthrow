#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <cstdint>

#include "../../lib/sole/sole.hpp"

#include "../component/component.hpp"

// Kinda like GameObject - https://docs.unity3d.com/ScriptReference/GameObject.html

class Entity {
public:
	Entity(sole::uuid uuid, std::string name);

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	std::shared_ptr<T> addComponent() {
		auto c = std::make_shared<T>();
		T::getActiveComponents().push_back(c);
		_components.push_back(c);
		return c;
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	std::shared_ptr<T> getComponent() {
		for (std::shared_ptr<IComponent> c : _components) {
			std::shared_ptr<T> com = std::dynamic_pointer_cast<T>(c);
			if (com)
				return com;
		}
		return std::shared_ptr<T>();
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	void removeComponent() {
		std::shared_ptr<T> com;
		for (std::shared_ptr<IComponent> c : _components) {
			com = std::dynamic_pointer_cast<T>(c);
			if (com)
				break;
		}
		if (!com)
			return;

		auto list = T::getActiveComponents();
		for (auto it = list.begin(); it != list.end(); it++)
			if (*it == com) {
				list.erase(it);
				return;
			}
	}

	inline const sole::uuid& getUUID() { return _uuid; }
	inline const std::string& getName() { return _name; }
	inline std::vector<std::shared_ptr<IComponent>>& getComponents() { return _components; }

protected:
	sole::uuid _uuid;
	std::string _name;
	std::vector<std::shared_ptr<IComponent>> _components;
};
