#pragma once

#include <SDL2/SDL.h>
#include "lib/glad.h"
#include <glm/glm.hpp>
#include <exception>
#include <memory>
#include <cmath>

#include "io/texturemanager.hpp"
#include "io/meshloader.hpp"
#include "world/world.hpp"

#include "world/system/physics.hpp"
#include "world/system/render.hpp"
#include "world/system/imgui.hpp"

class Engine {
public:
	static Engine& getInstance() {
		static Engine instance;
		return instance;
	}

	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

	int run(bool vsync);

	inline std::shared_ptr<TextureManager> getTextureManager() { return _textureManager; }
	inline std::shared_ptr<MeshLoader> getMeshLoader() { return _meshLoader; }

	inline World& getWorld() { return _world; }

private:
	uint32_t _width = 1280;
	uint32_t _height = 720;
	bool _vsync = true;

	float _speed = 5.0f;
	float _fov = 80.0f;

	float _yaw = 0; // +Z (at 0, 0, 0)
	float _pitch = 0.0f;

	glm::vec3 _position = glm::vec3(0, 0, -2);

	glm::mat4 _projection;
	glm::mat4 _view;

	bool _quit;
	SDL_Window* _window;
	SDL_GLContext _context;

	std::shared_ptr<TextureManager> _textureManager;
	std::shared_ptr<MeshLoader> _meshLoader;
	World _world;

	std::unique_ptr<PhysicsSystem> _physicsSystem;
	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<ImGuiSystem> _imGuiSystem;


	Engine() {}
	virtual ~Engine();

	void _init(bool vsync);
	void _initSDL();
	void _initGL();
	void _initImGui();

	void _resolutionChanged();
	// void _updateMovement(float delta, bool updateCamera);
};
