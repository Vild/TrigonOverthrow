#pragma once
#include "renderpass.hpp"
#include "../../gl/simplemesh.hpp"

class BulletDebugRenderPass : public RenderPass
{
public:
	BulletDebugRenderPass();
	virtual ~BulletDebugRenderPass();

	// Inherited via RenderPass
	virtual void registerImGui() override;
	virtual std::string name() override;
	virtual void render(World & world) override;
	virtual void resize(unsigned int width, unsigned int height) override;
private:
	SimpleMesh wireFrame;
};