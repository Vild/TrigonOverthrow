#pragma once
#include "renderpass.hpp"
#include "../../gl/simplemesh.hpp"

class BulletDebugRenderPass : public RenderPass {
public:
	BulletDebugRenderPass();
	virtual ~BulletDebugRenderPass();

	// Inherited via RenderPass
	void registerImGui() final;
	virtual std::string name() override;
	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;

private:
	SimpleMesh wireFrame;
	bool enable;
};