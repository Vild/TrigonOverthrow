// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "engine.hpp"

#include "lib/imgui.h"
#include "lib/imgui_impl_sdl_gl3.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <cmath>
#include <cstdio>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "world/component/lookatcomponent.hpp"
#include "world/component/cameracomponent.hpp"

#include "world/system/inputsystem.hpp"
#include "world/system/physicssystem.hpp"
#include "world/system/imguisystem.hpp"
#include "world/system/lookatsystem.hpp"
#include "world/system/camerasystem.hpp"
#include "world/system/particlesystem.hpp"
#include "world/system/buttonsystem.hpp"
#include "world/system/gunsystem.hpp"
#include "world/system/lifesystem.hpp"
#include "world/system/bulletphysicssystem.hpp"
#include "world/system/projectilesystem.hpp"
#include "world/system/dynamicmodelsystem.hpp"


#include "world/renderpass/geometryrenderpass.hpp"
#include "world/renderpass/ssaorenderpass.hpp"
#include "world/renderpass/gaussianrenderpass.hpp"
#include "world/renderpass/lightingrenderpass.hpp"
#include "world/renderpass/particlerenderpass.hpp"
#include "world/renderpass/textrenderpass.hpp"
#include "world/renderpass/bulletdebugrenderpass.hpp"

#include "state/ingamestate.hpp"
#include "state/mainmenustate.hpp"

Engine::~Engine() {
	_systems.clear();
	_states.clear();

	//rmt_UnbindOpenGL();
	//rmt_DestroyGlobalInstance(rmt);

	TTF_Quit();
	Mix_Quit();
	IMG_Quit();

	ImGui_ImplSdlGL3_Shutdown();

	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

int Engine::run(bool vsync) {
	_init(vsync);
	_quit = false;
	uint32_t lastTime = SDL_GetTicks();

	while (!_quit) {
		rmt_ScopedCPUSample(GameLoop, RMTSF_None);
		rmt_ScopedOpenGLSample(GameLoop);
		if (*_nextState != std::type_index(typeid(Engine))) {
			State* prev = getStatePtr();
			*_currentState = *_nextState;
			*_nextState = std::type_index(typeid(Engine));
			State* next = getStatePtr();
			if (next)
				next->onEnter(prev);

			if (prev)
				prev->onLeave(next);

			if (!next)
				break;
		}
		{
			rmt_ScopedCPUSample(EventHandling, RMTSF_None);
			rmt_ScopedOpenGLSample(EventHandling);
			SDL_Event event;
			ImGuiIO& io = ImGui::GetIO();
			while (SDL_PollEvent(&event)) {
				ImGui_ImplSdlGL3_ProcessEvent(&event);
				switch (event.type) {
				case SDL_QUIT:
					_quit = true;
					break;
				case SDL_KEYDOWN:
					if (io.WantCaptureKeyboard)
						break;
					switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						_quit = true;
						break;
					default:
						break;
					}
					break;

				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
						_width = event.window.data1;
						_height = event.window.data2;
						_system_resize(_width, _height);

						World& world = getState().getWorld();
						for (std::unique_ptr<Entity>& entity : world.getEntities()) {
							CameraComponent* cc = entity->getComponent<CameraComponent>();
							if (!cc)
								continue;
							cc->aspect = (_width * 1.0) / _height;
							cc->recalculateProjectionMatrix();
						}
					}
					break;
				default:
					break;
				}
			}
			ImGui_ImplSdlGL3_NewFrame(_window);

			{
				ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiSetCond_Always);
				ImGui::SetNextWindowSize(ImVec2(384, 32), ImGuiSetCond_Once);
				ImGui::Begin("Info panel", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
		}

		{
			rmt_ScopedCPUSample(WorldTick, RMTSF_None);
			rmt_ScopedOpenGLSample(WorldTick);

			uint32_t curTime = SDL_GetTicks();
			float delta = (curTime - lastTime) / 1000.0f;
			lastTime = curTime;

			_hidInput->update();

			_system_tick(delta);
			std::vector<std::unique_ptr<Entity>>& entities = getState().getWorld().getEntities();
			// Rigidbody entities in bulletphysicssystem.
			for (std::unique_ptr<Entity>& entity : entities) {
				auto rgbComponent = entity->getComponent<RigidBodyComponent>();
				if (!rgbComponent)
					continue;
				if (entity->isDead())
					getSystem<BulletPhysicsSystem>()->removeRigidBody(rgbComponent);
			}
			// Entities in world.
			entities.erase(
				std::remove_if(
					entities.begin(),
					entities.end(),
					[](const std::unique_ptr<Entity>& e) -> bool { return e->isDead(); }
				),
				entities.end()
			);
		}

		{
			rmt_ScopedCPUSample(ImGuiRender, RMTSF_None);
			rmt_ScopedOpenGLSample(ImGuiRender);
			// static bool debug = true;
			// ImGui::ShowMetricsWindow(&debug);

			ImGui::Render();
		}
		{
			rmt_ScopedCPUSample(SwapWindow, RMTSF_None);
			rmt_ScopedOpenGLSample(SwapWindow);
			SDL_GL_SwapWindow(_window);
		}
	}
	return 0;
}

void Engine::_init(bool vsync) {
	_vsync = vsync;
	_initSDL();
	_initGL();
	_initImGui();

	rmt_CreateGlobalInstance(&rmt);
	rmt_BindOpenGL();

	rmt_ScopedCPUSample(Initialization, RMTSF_None);
	{
		rmt_ScopedCPUSample(InitializingHelpers, RMTSF_None);
		_textureManager = std::make_shared<TextureManager>(); // TODO: Move to own function?
		_meshLoader = std::make_shared<MeshLoader>();
		_mapLoader = std::make_shared<MapLoader>();
		_hidInput = std::make_shared<HIDInput>();
		_textFactory = std::make_shared<TextFactory>("assets/fonts/font.png");
	}

	{
		rmt_ScopedCPUSample(InitializingECS, RMTSF_None);
		_currentState = std::make_unique<std::type_index>(std::type_index(typeid(nullptr)));
		_nextState = std::make_unique<std::type_index>(std::type_index(typeid(Engine)));

		_setupSystems();
		_states[std::type_index(typeid(nullptr))] = std::unique_ptr<State>();
		_states[std::type_index(typeid(InGameState))] = std::make_unique<InGameState>();
		_states[std::type_index(typeid(MainMenuState))] = std::make_unique<MainMenuState>();
		setState<InGameState>();
	}
}

void Engine::_initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "SDL_Init: Failed to init!\n");
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		throw "Failed to load SDL2";
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		fprintf(stderr, "IMG_Init: Failed to init, requires PNG support!\n");
		fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());
		throw "Failed to load SDL2_image";
	}

	if ((Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG) != MIX_INIT_OGG) {
		fprintf(stderr, "Mix_Init: Failed to init, requires ogg support!\n");
		fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
		throw "Failed to load SDL2_mixer";
	}

	if (TTF_Init() < 0) {
		fprintf(stderr, "TTF_GetError: Failed to init!\n");
		fprintf(stderr, "TTF_GetError: %s\n", TTF_GetError());
		throw "Failed to load SDL2_ttf";
	}

	_window =
		SDL_CreateWindow("Trigon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _width, _height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (!_window)
		throw "Failed to create window";
}

void Engine::_initGL() {
	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	_context = SDL_GL_CreateContext(_window);
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		throw "Failed to init GLAD";

	SDL_GL_SetSwapInterval(_vsync);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_PROGRAM_POINT_SIZE);

	glViewport(0, 0, _width, _height);
}

void Engine::_initImGui() {
	ImGui_ImplSdlGL3_Init(_window);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("assets/fonts/DroidSans.ttf", 18.0f);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.83f, 0.95f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.39f, 0.80f, 0.80f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.27f, 0.27f, 0.87f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.25f, 0.75f, 0.75f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.55f, 0.55f, 0.67f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.75f, 0.75f, 0.67f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.49f, 0.49f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.49f, 0.49f, 0.60f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.18f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.47f, 0.47f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.33f, 0.33f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.14f, 0.42f, 0.42f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.18f, 0.55f, 0.55f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.44f, 0.81f, 0.81f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.33f, 0.78f, 0.78f, 0.60f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.18f, 0.55f, 0.55f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.44f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.25f, 0.74f, 0.74f, 0.40f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.25f, 0.74f, 0.74f, 0.60f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.25f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.53f, 0.84f, 0.84f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.22f, 0.67f, 0.67f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.53f, 0.84f, 0.84f, 0.40f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.84f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.84f, 0.84f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.13f, 0.40f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.09f, 0.27f, 0.27f, 0.67f);
}

void Engine::_setupSystems() {
	// Pure systems
	_systems.push_back(std::make_unique<ImGuiSystem>());
	_systems.push_back(std::make_unique<InputSystem>());
	_systems.push_back(std::make_unique<PhysicsSystem>());
	_systems.push_back(std::make_unique<BulletPhysicsSystem>());
	_systems.push_back(std::make_unique<LookAtSystem>());
	_systems.push_back(std::make_unique<CameraSystem>());
	_systems.push_back(std::make_unique<ButtonSystem>());
	_systems.push_back(std::make_unique<GunSystem>());
	_systems.push_back(std::make_unique<ParticleSystem>());
	_systems.push_back(std::make_unique<ProjectileSystem>());
	_systems.push_back(std::make_unique<DynamicModelSystem>());
	_systems.push_back(std::make_unique<LifeSystem>());

	// Render passes
	{
		std::unique_ptr<GeometryRenderPass> geometry = std::make_unique<GeometryRenderPass>();
		std::unique_ptr<SSAORenderSystem> ssao = std::make_unique<SSAORenderSystem>();
		std::unique_ptr<GaussianRenderPass> gaussian = std::make_unique<GaussianRenderPass>();
		std::unique_ptr<LightingRenderPass> lighting = std::make_unique<LightingRenderPass>();
		std::unique_ptr<ParticleRenderPass> particles = std::make_unique<ParticleRenderPass>();
		std::unique_ptr<TextRenderPass> text = std::make_unique<TextRenderPass>();

		ssao->attachInputTexture(SSAORenderSystem::InputAttachments::PositionMap, geometry->getAttachment(GeometryRenderPass::Attachment::position))
			.attachInputTexture(SSAORenderSystem::InputAttachments::NormalMap, geometry->getAttachment(GeometryRenderPass::Attachment::normal));

		gaussian->attachInputTexture(GaussianRenderPass::InputAttachments::Image, ssao->getAttachment(SSAORenderSystem::Attachments::OcclusionMap));

		lighting->attachInputTexture(LightingRenderPass::InputAttachment::position, geometry->getAttachment(GeometryRenderPass::Attachment::position))
			.attachInputTexture(LightingRenderPass::InputAttachment::normal, geometry->getAttachment(GeometryRenderPass::Attachment::normal))
			.attachInputTexture(LightingRenderPass::InputAttachment::diffuseSpecular, geometry->getAttachment(GeometryRenderPass::Attachment::diffuseSpecular))
			.attachInputTexture(LightingRenderPass::InputAttachment::depth, geometry->getAttachment(GeometryRenderPass::Attachment::depth))
			.attachInputTexture(LightingRenderPass::InputAttachment::OcclusionMap, gaussian->getAttachment(GaussianRenderPass::Attachments::BlurredImage));

		{ // GIT-GUD: fixed in velocity and position to be output instead.
			auto particleSystem = getSystem<ParticleSystem>();
			auto _gbuffer = particleSystem->getGBuffers();

			(*particles)
				.attachInputTexture(ParticleRenderPass::InputAttachment::position, _gbuffer->getAttachment(ParticleSystem::Attachment::inPosition))
				.attachInputTexture(ParticleRenderPass::InputAttachment::velocity, _gbuffer->getAttachment(ParticleSystem::Attachment::inVelocity));
		}

		_systems.push_back(std::move(geometry));
		_systems.push_back(std::move(ssao));
		_systems.push_back(std::move(gaussian));
		_systems.push_back(std::move(lighting));
		_systems.push_back(std::move(particles));
		_systems.push_back(std::move(text));
		_systems.push_back(std::make_unique<BulletDebugRenderPass>());
	}
}

void Engine::_system_tick(float delta) {
	World& world = getState().getWorld();

	for (const std::unique_ptr<System>& system : _systems)
		system->update(world, delta);
}

void Engine::_system_resize(unsigned int width, unsigned int height) {
	for (std::unique_ptr<System>& system : _systems) {
		RenderPass* rp = dynamic_cast<RenderPass*>(system.get());
		if (!rp)
			continue;
		rp->resize(width, height);
	}
}
