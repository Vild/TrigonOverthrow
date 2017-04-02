#pragma once

#include <string>
#include <type_traits>
#include <memory>

#include "../component/component.hpp"

// Kinda like GameObject - https://docs.unity3d.com/ScriptReference/GameObject.html

class Entity {
public:
	virtual void update(float delta);
	inline const std::string& name() { return _name; }

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	std::shared_ptr<T> addComponent() {
		auto c = std::make_shared<T>(*this);
		T::getActiveComponents().push_back(c);
		return c;
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	std::shared_ptr<T> getComponent() {
		for (std::shared_ptr<T> c : T::getActiveComponents())
			if (c->getEntity() == *this)
				return c;
		return std::shared_ptr<T>();
	}

	template <typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
	void removeComponent() {
		auto list = T::getActiveComponents();
		for (auto it = list.begin(); it != list.end(); it++)
			if (it->getEntity() == *this) {
				list.erase(it);
				return;
			}
	}

protected:
	std::string _name;
};
