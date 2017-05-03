#include "bulletdebugrenderpass.hpp"
#include <glm/gtx/transform.hpp>
#include <vector>

#include "../component/transformcomponent.hpp"
#include "../component/rigidbodycomponent.hpp"
#include "../component/cameracomponent.hpp"
#include "../../engine.hpp"

BulletDebugRenderPass::BulletDebugRenderPass()
{
	enable = false;


	wireFrame.setDrawMode(GL_LINES)
		// Bottom
		.addVertex({ -1,-1,-1 }).addVertex({  1,-1,-1 }) // TOP
		.addVertex({ -1,-1, 1 }).addVertex({  1,-1, 1 }) // BOTTOM
		.addVertex({ -1,-1,-1 }).addVertex({ -1,-1, 1 }) // LEFT
		.addVertex({  1,-1,-1 }).addVertex({  1,-1, 1 }) // RIGHT
		// TOP
		.addVertex({ -1, 1,-1 }).addVertex({  1, 1,-1 }) // TOP
		.addVertex({ -1, 1, 1 }).addVertex({  1, 1, 1 }) // BOTTOM
		.addVertex({ -1, 1,-1 }).addVertex({ -1, 1, 1 }) // LEFT
		.addVertex({  1, 1,-1 }).addVertex({  1, 1, 1 }) // RIGHT
		// SIDES
		.addVertex({ -1,-1,-1 }).addVertex({ -1, 1,-1 }) // BACK LEFT
		.addVertex({  1,-1,-1 }).addVertex({  1, 1,-1 }) // BACK RIGHT
		.addVertex({ -1,-1, 1 }).addVertex({ -1, 1, 1 }) // FRONT LETT
		.addVertex({  1,-1, 1 }).addVertex({  1, 1, 1 }) // FRONT RIGHT		
	.finalize();


	_gbuffer = std::make_shared<GBuffer>(0);

	(*(_shader = std::make_shared<ShaderProgram>()))
		.attach("assets/shaders/bulletdebug.vert", ShaderType::vertex)
		.attach("assets/shaders/bulletdebug.frag", ShaderType::fragment)
	.finalize();

	_shader->bind()
		.addUniform("u_view")
		.addUniform("u_projection");
}

BulletDebugRenderPass::~BulletDebugRenderPass()
{
}

void BulletDebugRenderPass::registerImGui()
{
	ImGui::Checkbox("Enable", &enable);
}

inline std::string BulletDebugRenderPass::name()
{
	return "BulletDebugRenderPass";
}

void BulletDebugRenderPass::render(World & world)
{
	rmt_ScopedCPUSample(BulletDebugRenderPass, RMTSF_None);
	rmt_ScopedOpenGLSample(BulletDebugRenderPass);
	if (!enable) return;

	auto camera = Engine::getInstance().getCamera()->getComponent<CameraComponent>();


	//glClear(GL_DEPTH_BUFFER_BIT);

	_shader->bind()
		.setUniform("u_view", camera->viewMatrix)
		.setUniform("u_projection", camera->projectionMatrix);

	static std::vector<glm::mat4> instances;
	instances.clear();

	for (auto& entity : world.getEntities())
	{
		auto rigidbody = entity->getComponent<RigidBodyComponent>();
		if (!rigidbody) continue;

		auto t = rigidbody->getRigidBody()->getWorldTransform();
		glm::vec3 pos = cast(t.getOrigin());
		glm::quat rot = cast(t.getRotation());
		glm::vec3 scale = rigidbody->getHitboxHalfSize();

		glm::mat4 m = glm::translate(pos) * glm::mat4_cast(rot) * glm::scale(scale);
		
		instances.push_back(m);
	}

	wireFrame.draw(instances);
}

void BulletDebugRenderPass::resize(unsigned int width, unsigned int height)
{
}
