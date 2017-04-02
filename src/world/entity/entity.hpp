#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <vector>

#include "../component/component.hpp"

// Kinda like GameObject - https://docs.unity3d.com/ScriptReference/GameObject.html

class Entity {
public:

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
			std::shared_ptr<T> com = (std::shared_ptr<T>)c;
			if (com)
				return com;
		}
		return std::shared_ptr<T>();
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	void removeComponent() {
		std::shared_ptr<T> com;
		for (std::shared_ptr<IComponent> c : _components) {
			com = (std::shared_ptr<T>)c;
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

	inline const std::string & getName() { return _name; }
	inline std::vector<std::shared_ptr<IComponent>> & getComponents() { return _components; }

protected:
	std::string _name;
	std::vector<std::shared_ptr<IComponent>> _components;
};
