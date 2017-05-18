#pragma once

#include <SDL2/SDL.h>
#include "lib/glad.h"
#include <glm/glm.hpp>
#include <exception>
#include <memory>
#include <cmath>
#include <typeinfo>
#include <typeindex>
#include <map>

#include "io/texturemanager.hpp"
#include "io/meshloader.hpp"
#include "io/hidinput.hpp"
#include "io/textfactory.hpp"
#include "io/maploader.hpp"
#include "io/jsonloader.hpp"
#include "io/audiomanager.hpp"

#include "world/world.hpp"
#include "world/system/system.hpp"
#include "state/state.hpp"

class Engine {
public:
	typedef std::unique_ptr<State> (*makeState_f)();
	static Engine& getInstance() {
		static Engine instance;
		return instance;
	}

	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

	int run(bool vsync);

	inline unsigned int& getWidth() { return _width; }
	inline unsigned int& getHeight() { return _height; }
	inline SDL_Window* getWindow() { return _window; }

	inline ImFont* getDefaultFont() { return _defaultFont; }
	inline ImFont* getBigFont() { return _bigFont; }
	inline ImFont* getMediumFont() { return _mediumFont; }

	inline std::shared_ptr<TextureManager> getTextureManager() { return _textureManager; }
	inline std::shared_ptr<MeshLoader> getMeshLoader() { return _meshLoader; }
	inline std::shared_ptr<HIDInput> getHIDInput() { return _hidInput; }
	inline std::shared_ptr<TextFactory> getTextFactory() { return _textFactory; }
	inline std::shared_ptr<MapLoader> getMapLoader() { return _mapLoader; }
	inline std::shared_ptr<JSONLoader> getJSONLoader() { return _jsonLoader; }
	inline std::shared_ptr<AudioManager> getAudioManager() { return _audioManager; }

	inline std::vector<std::unique_ptr<System>>& getSystems() { return _systems; }

	template <typename T>
	T* getSystem();

	inline Entity* getCamera() {
		State* s = getStatePtr();
		if (!s)
			return nullptr;
		return s->getCamera();
	}

	template <typename T>
	inline std::map<std::type_index, makeState_f>& getStates() {
		return _states;
	}

	inline State& getState() { return *_currentState; }
	inline State* getStatePtr() { return _currentState.get(); }

	template <typename T>
	inline void setState() {
		*_nextState = std::type_index(typeid(T));
	}

	inline void quit() { *_nextState = std::type_index(typeid(nullptr)); }

	inline bool& getPause() { return _pause; }

private:
	unsigned int _width = 1280;
	unsigned int _height = 720;
	bool _vsync = true;

	bool _quit;
	SDL_Window* _window;
	SDL_GLContext _context;

	ImFont* _defaultFont;
	ImFont* _bigFont;
	ImFont* _mediumFont;

	std::shared_ptr<TextureManager> _textureManager;
	std::shared_ptr<MeshLoader> _meshLoader;
	std::shared_ptr<HIDInput> _hidInput;
	std::shared_ptr<TextFactory> _textFactory;
	std::shared_ptr<MapLoader> _mapLoader;
	std::shared_ptr<JSONLoader> _jsonLoader;
	std::shared_ptr<AudioManager> _audioManager;

	std::vector<std::unique_ptr<System>> _systems;

	std::unique_ptr<State> _currentState;
	std::unique_ptr<std::type_index> _nextState;
	std::map<std::type_index, makeState_f> _states;
	bool _pause = false;

	Engine() {}
	virtual ~Engine();

	void _init(bool vsync);
	void _initSDL();
	void _initGL();
	void _initImGui();

	void _setupSystems();

	void _system_tick(float delta);
	void _system_resize(unsigned int width, unsigned int height);

	static void _glDebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
};

template <typename T>
T* Engine::getSystem() {
	T* ptr = nullptr;

	size_t i = 0, size = _systems.size();
	while (ptr == nullptr && i < size) {
		ptr = dynamic_cast<T*>(_systems[i++].get());
	}

	if (ptr == nullptr)
		throw "system does not exist";

	return ptr;
}
