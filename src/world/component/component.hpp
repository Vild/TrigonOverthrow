#pragma once

#include <string>
#include <vector>
#include <memory>

class Entity;

/// Used to identifying a Component
class IComponent {
public:
	IComponent(Entity& entity) : _entity(entity) {}

	inline Entity& getEntity() { return _entity; }

protected:
	Entity& _entity;
};

/// Used for extending and getting all the active components
template <typename T>
class Component : public IComponent {
public:
	static inline std::vector<std::shared_ptr<T>> & getActiveComponents() { return _activeComponents; }

private:
	static std::vector<std::shared_ptr<T>> _activeComponents;
};

template <typename T>
std::vector<std::shared_ptr<T>> Component<T>::_activeComponents;
