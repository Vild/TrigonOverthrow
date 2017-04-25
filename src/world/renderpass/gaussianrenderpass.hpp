#pragma once
#include "renderpass.hpp"
#include "../../gl/mesh.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/texture.hpp"

class GaussianRenderPass : public RenderPass {
public:
	GaussianRenderPass();
	virtual ~GaussianRenderPass();

	enum InputAttachments { Image };
	enum Attachments { BlurredImage };

	virtual void render(World& world) override;
	virtual void resize(unsigned int width, unsigned int height) override;

	virtual void registerImGui() override;
	virtual std::string name() { return "GaussianRenderPass"; };

private:
	std::unique_ptr<Mesh> fsQuad;
	std::shared_ptr<GBuffer> altBuffer;

	int kernelSize;
	float sigma;

	std::unique_ptr<Mesh> makeFsQuad();

	void generateKernel();
};