#pragma once

#include <SDL2/SDL.h>
#include "lib/glad.h"
#include <glm/glm.hpp>
#include <exception>
#include <memory>
#include <cmath>

#include "io/texturemanager.hpp"
#include "io/meshloader.hpp"
#include "io/hidinput.hpp"
#include "world/world.hpp"

#include "world/system/inputsystem.hpp"
#include "world/system/physicssystem.hpp"
#include "world/system/rendersystem.hpp"
#include "world/system/imguisystem.hpp"
#include "world/system/lookatsystem.hpp"

#include "world/entity/cameraentity.hpp"

class Engine {
public:
	static Engine& getInstance() {
		static Engine instance;
		return instance;
	}

	Engine(Engine const&) = delete;
	void operator=(Engine const&) = delete;

	int run(bool vsync);

	inline uint32_t& getWidth() { return _width; }
	inline uint32_t& getHeight() { return _height; }
	inline bool& getUpdateCamera() { return _updateCamera; }
	inline SDL_Window* getWindow() { return _window; }

	inline std::shared_ptr<TextureManager> getTextureManager() { return _textureManager; }
	inline std::shared_ptr<MeshLoader> getMeshLoader() { return _meshLoader; }
	inline std::shared_ptr<HIDInput> getHIDInput() { return _hidInput; }

	inline World& getWorld() { return _world; }
	inline std::shared_ptr<CameraEntity> getCamera() { return _camera; }

private:
	uint32_t _width = 1280;
	uint32_t _height = 720;
	bool _updateCamera;
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
	std::shared_ptr<HIDInput> _hidInput;

	World _world;
	std::shared_ptr<CameraEntity> _camera;

	std::unique_ptr<InputSystem> _inputSystem;
	std::unique_ptr<PhysicsSystem> _physicsSystem;
	std::unique_ptr<RenderSystem> _renderSystem;
	std::unique_ptr<ImGuiSystem> _imGuiSystem;
	std::unique_ptr<LookAtSystem> _lookAtSystem;

	Engine() {}
	virtual ~Engine();

	void _init(bool vsync);
	void _initSDL();
	void _initGL();
	void _initImGui();

	void _resolutionChanged();
	// void _updateMovement(float delta, bool updateCamera);
};
