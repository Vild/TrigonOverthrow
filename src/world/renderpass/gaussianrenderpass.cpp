#include "gaussianrenderpass.hpp"
#include "../../engine.hpp"

GaussianRenderPass::GaussianRenderPass()
{
	auto& engine = Engine::getInstance();
	int width = engine.getWidth();
	int height = engine.getHeight();

	(*(_gbuffer = std::make_shared<GBuffer>()))
		.attachTexture(0, width, height, GL_RGB, GL_UNSIGNED_BYTE, 3)
		.finalize();

	//(*(altBuffer = std::make_shared<GBuffer>()))
	//	.attachTexture(0, width, height, GL_RGB, GL_UNSIGNED_BYTE, 3)
	//	.finalize();

	//(*(_shader = std::make_shared<ShaderProgram>()))
	//	.attach("assets/shaders/gaussian.vert", ShaderType::vertex)
	//	.attach("assets/shaders/gaussian.frag", ShaderType::fragment)
	//	.finalize();

	//_shader->bind()
	//	.addUniform("image")
	//	.setUniform("image", 0);

	//fsQuad = makeFsQuad();

	//setPasses(1);
}

std::unique_ptr<Mesh> GaussianRenderPass::makeFsQuad()
{
	std::vector<Vertex> vertices = {
		Vertex{ glm::vec3{ -1, 1, 0 }, glm::vec3{ 0, 0, -1 },{ 1.0, 1.0, 1.0 },{ 0, 1 } },	//
		Vertex{ glm::vec3{ 1, 1, 0 }, glm::vec3{ 0, 0, -1 },{ 1.0, 1.0, 1.0 },{ 1, 1 } },	//
		Vertex{ glm::vec3{ 1, -1, 0 }, glm::vec3{ 0, 0, -1 },{ 1.0, 1.0, 1.0 },{ 1, 0 } },	//
		Vertex{ glm::vec3{ -1, -1, 0 }, glm::vec3{ 0, 0, -1 },{ 1.0, 1.0, 1.0 },{ 0, 0 } }, //
	};
	std::vector<GLuint> indicies = { 0, 2, 1, 2, 0, 3 };

	std::unique_ptr<Mesh> plane = std::make_unique<Mesh>(vertices, indicies);
	plane->addBuffer("m", [](GLuint id) {
		glm::mat4 mData = glm::mat4(1);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), glm::value_ptr(mData), GL_STATIC_DRAW); // Will only be uploaded once

		for (int i = 0; i < 4; i++) {
			glEnableVertexAttribArray(ShaderAttributeID::m + i);
			glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
			glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}).finalize();

	return std::move(plane);
}

void GaussianRenderPass::registerImGui()
{
	bool dirty = false;

	dirty |= ImGui::DragInt("Passes", &passes, 2, 1, 61);

	if (dirty)
	{
		passes += 1 - passes % 2;
	}
}

inline void GaussianRenderPass::setPasses(int passes) 
{
	this->passes = passes + 1 - passes % 2;
}


void GaussianRenderPass::render(World & world)
{
	/*static std::shared_ptr<GBuffer> buffers[] = { _gbuffer, altBuffer };
	static std::shared_ptr<Texture> images[] = { buffers[0]->getAttachment(0), buffers[1]->getAttachment(0) };

	_shader->bind();
	fsQuad->render();

	bool ping = true;
	for (int i = 1; i < passes; i++)
	{
		buffers[ping]->bind();
		images[!ping]->bind();
		fsQuad->render();
		ping = !ping;
	}*/
}

void GaussianRenderPass::resize(unsigned int width, unsigned int height)
{
}
