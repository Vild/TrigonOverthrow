#pragma once
#include "renderpass.hpp"
#include "../../gl/mesh.hpp"
#include "../../gl/gbuffer.hpp"
#include "../../gl/texture.hpp"

class GaussianRenderPass : public RenderPass {
public:
	GaussianRenderPass();
	virtual ~GaussianRenderPass();

	enum InputAttachments { image };
	enum Attachments { blurredImage };

	void render(World& world) final;
	void resize(unsigned int width, unsigned int height) final;

	void registerImGui() final;
	inline std::string name() final { return "GaussianRenderPass"; };

private:
	std::unique_ptr<Mesh> fsQuad;
	std::shared_ptr<GBuffer> altBuffer;

	int kernelSize;
	float sigma;

	std::unique_ptr<Mesh> makeFsQuad();

	void generateKernel();
};
