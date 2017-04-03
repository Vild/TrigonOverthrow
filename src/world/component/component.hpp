#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../../lib/imgui.h"

/// Used to identifying a Component
struct IComponent {
	virtual std::string name() = 0;

	virtual void registerImGui() = 0;
};

/// Used for extending and getting all the active components
template <typename T>
struct Component : public IComponent {
public:
	static inline std::vector<std::shared_ptr<T>>& getActiveComponents() { return _activeComponents; }

	virtual void registerImGui() {}

private:
	static std::vector<std::shared_ptr<T>> _activeComponents;
};

template <typename T>
std::vector<std::shared_ptr<T>> Component<T>::_activeComponents;
