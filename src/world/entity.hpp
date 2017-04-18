#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <cstdint>

#include "../lib/sole/sole.hpp"

#include "component/component.hpp"

// Kinda like GameObject - https://docs.unity3d.com/ScriptReference/GameObject.html

class Entity {
public:
	Entity(sole::uuid uuid, std::string name);

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	std::unique_ptr<T>& addComponent() {
		T::getActiveComponents().push_back(std::make_unique<T>());
		auto& c = T::getActiveComponents().back();
		_components.push_back(c);
		return c;
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	std::unique_ptr<T>& addComponent(std::unique_ptr<T> c) {
		T::getActiveComponents().push_back(std::move(c));

		auto& c = T::getActiveComponents().back();
		_components.push_back(c);
		return c;
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	std::unique_ptr<T>& getComponent() {
		for (std::unique_ptr<IComponent>& c : _components) {
			T* com = dynamic_pointer<T>(c.get());
			if (com)
				return com;
		}
		return std::unique_ptr<T>();
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
			auto com = std::dynamic_pointer_cast<T>(*it);
			if (!com)
				continue;
			_components.erase(it);
			break;
		}
	}

	//virtual void registerImGui() = 0;

	inline const sole::uuid& getUUID() { return _uuid; }
	inline const std::string& getName() { return _name; }
	inline std::vector<std::unique_ptr<IComponent>&>& getComponents() { return _components; }

protected:
	sole::uuid _uuid;
	std::string _name;
	std::vector<std::unique_ptr<IComponent>&> _components;
};
