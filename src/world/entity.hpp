#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <vector>
#include <cstdint>
#include <map>


#include "component/component.hpp"

// Kinda like GameObject - https://docs.unity3d.com/ScriptReference/GameObject.html

class World;
class State;

class Entity {
public:
	/// Don't call this one directly! Always use world->addEntity
	Entity(World& world, std::string name);
	virtual ~Entity();

	template <typename T, typename... Args, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* addComponent(Args... args) {
		auto ret = components.insert_or_assign(typeid(T), std::make_unique<T>(args...));

		_world._activeComponents[typeid(T)].push_back(this);

		return static_cast<T*>(ret.first->second.get());
	}

	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	T* getComponent() {
		T* component = nullptr;
		auto it = components.find(typeid(T));

		if (it != components.end())
			component = static_cast<T*>(it->second.get());

		return component;
	}

	template <typename T, typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
	void removeComponent() {
		components.erase(typeid(T));
		auto& ents = _world._activeComponents[typeid(T)];
		ents.erase(std::find(ents.begin(), ents.end(), this));
	}

	typedef void (*registerImGui_f)(Entity& self, State& state);
	registerImGui_f registerImGui = nullptr;

	inline std::string& getName() { return _name; }
	inline void makeDead() { _dead = true; }
	inline bool isDead() { return _dead; }
	inline std::map<std::type_index, std::unique_ptr<Component>>& getComponents() { return components; }
	inline bool& getHide() { return _hide; }

private:
	World& _world;
	std::string _name;
	std::map<std::type_index, std::unique_ptr<Component>> components;

	bool _dead = false;
	bool _hide = false;
};
