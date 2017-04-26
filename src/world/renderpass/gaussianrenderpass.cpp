#include "gaussianrenderpass.hpp"
#include "../../engine.hpp"
#include <functional>

GaussianRenderPass::GaussianRenderPass() {
	auto& engine = Engine::getInstance();
	int width = engine.getWidth();
	int height = engine.getHeight();

	(*(_gbuffer = std::make_shared<GBuffer>())).bind().attachTexture(Attachments::BlurredImage, std::make_shared<Texture>(width, height, GL_RGB)).finalize();

	(*(altBuffer = std::make_shared<GBuffer>())).bind().attachTexture(Attachments::BlurredImage, std::make_shared<Texture>(width, height, GL_RGB)).finalize();

	(*(_shader = std::make_shared<ShaderProgram>()))
		.attach("assets/shaders/gaussian.vert", ShaderType::vertex)
		.attach("assets/shaders/gaussian.frag", ShaderType::fragment)
		.finalize();

	_shader->bind().addUniform("kernel").addUniform("kernelSize").addUniform("horizontal").addUniform("image").setUniform("image", 0);

	fsQuad = makeFsQuad();

	kernelSize = 5;
	sigma = 1.0f;
	generateKernel();
}

GaussianRenderPass::~GaussianRenderPass() {}

std::unique_ptr<Mesh> GaussianRenderPass::makeFsQuad() {
	std::vector<Vertex> vertices = {
		Vertex{glm::vec3{-1, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 1}},	//
		Vertex{glm::vec3{1, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 1}},		//
		Vertex{glm::vec3{1, -1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 0}},	//
		Vertex{glm::vec3{-1, -1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 0}}, //
	};
	std::vector<GLuint> indicies = {0, 2, 1, 2, 0, 3};

	std::unique_ptr<Mesh> plane = std::make_unique<Mesh>(vertices, indicies);
	plane
		->addBuffer("m",
								[](GLuint id) {
									glm::mat4 mData = glm::mat4(1);
									glBindBuffer(GL_ARRAY_BUFFER, id);
									glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(mData), GL_STATIC_DRAW); // Will only be uploaded once

									for (int i = 0; i < 4; i++) {
										glEnableVertexAttribArray(ShaderAttributeID::m + i);
										glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
										glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
									}

									glBindBuffer(GL_ARRAY_BUFFER, 0);
								})
		.finalize();

	return std::move(plane);
}

void GaussianRenderPass::generateKernel() {
	std::vector<float> kernel;

	auto g = [&](int x) -> float {
		static float gMult = 1.0f / (sqrt(2.0f * glm::pi<float>()) * sigma);
		return gMult * pow(glm::e<float>(), -(pow(x, 2) / (2.0f * pow(sigma, 2))));
	};

	float kernelSum = 0;
	int halfWidth = (kernelSize + 1) / 2;
	for (int x = 0; x < halfWidth; x++) {
		float factor = g(x);
		kernelSum += factor * (x == 0 ? 1 : 2);
		kernel.push_back(factor);
	}

	for (auto& k : kernel) {
		k /= kernelSum;
	}

	_shader->bind().setUniformArray("kernel", kernel).setUniform("kernelSize", halfWidth);
}

void GaussianRenderPass::registerImGui() {
	bool dirty = false;

	dirty |= ImGui::DragInt("Kernel Size", &kernelSize, 1, 1, 64);
	dirty |= ImGui::DragFloat("Sigma", &sigma, 0.001, 0);

	if (dirty) {
		generateKernel();
	}
}

void GaussianRenderPass::render(World& world) {
	rmt_ScopedCPUSample(GaussianRenderPass, RMTSF_None);
	rmt_ScopedOpenGLSample(GaussianRenderPass);
	static std::shared_ptr<GBuffer> buffers[] = {_gbuffer, altBuffer};
	static std::shared_ptr<Texture> images[] = {buffers[0]->getAttachment(0), buffers[1]->getAttachment(0)};

	_shader->bind();

	bool ping = true;
	for (int i = 0; i < 2; i++) {
		_shader->setUniform("horizontal", ping);
		buffers[ping]->bind();
		fsQuad->render();
		images[ping]->bind();
		ping = !ping;
	}
}

void GaussianRenderPass::resize(unsigned int width, unsigned int height) {
	_gbuffer->getAttachments()[Attachments::BlurredImage]->resize(width, height, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE);
	_gbuffer->bind();
	glViewport(0, 0, width, height);

	altBuffer->getAttachments()[Attachments::BlurredImage]->resize(width, height, GL_RGB, GL_RGBA, GL_UNSIGNED_BYTE);
	altBuffer->bind();
	glViewport(0, 0, width, height);
}
