#include "textfactory.hpp"

#include "../engine.hpp"

TextFactory::TextFactory(const std::string& fontFile) {
	auto tm = Engine::getInstance().getTextureManager();
	_fontMap = tm->getTexture(fontFile);
	_fontMap->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST).setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	float w = 1.0 / 16.0;
	float h = w;

	for (int i = 0; i < 0x100; i++) {
		auto& info = _charInfos[i];
		info.pos = glm::vec2{w * (i % 16), h * (i / 16)};
		info.size = glm::vec2{w, h};
		// info.offset = glm::vec2{0, 0};
		info.xAdvanceAmount = w;
	}
}

TextFactory::~TextFactory() {
	_renderers.clear();
}

std::shared_ptr<TextRenderer> TextFactory::makeRenderer(const std::string& text, bool isStatic, unsigned int maxTextLength) {
	auto tr = std::make_shared<TextRenderer>(*this, text, isStatic, maxTextLength);
	_renderers.push_back(tr);
	return tr;
}

TextRenderer::TextRenderer(TextFactory& factory, const std::string& text, bool isStatic, unsigned int maxTextLength)
	: _factory(factory), _text(text), _isStatic(isStatic), _maxTextLength((isStatic ? text.size() : maxTextLength) * 2) {
	_renderingData.resize(_maxTextLength);
	_rebuild(false);
	const std::vector<Vertex> vertices = {
		Vertex{glm::vec3{0, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 0}}, //
		Vertex{glm::vec3{1, 1, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 0}}, //
		Vertex{glm::vec3{1, 0, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {0, 1}}, //
		Vertex{glm::vec3{0, 0, 0}, glm::vec3{0, 0, -1}, {1.0, 1.0, 1.0}, {1, 1}}, //
	};
	const std::vector<GLuint> indicies = {0, 2, 1, 2, 0, 3};
	_mesh = std::make_unique<Mesh>(vertices, indicies);
	_mesh
		->addBuffer(
			"renderInfo",
			[&](GLuint id) {
				glBindBuffer(GL_ARRAY_BUFFER, id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(CharRenderInfo) * _renderingData.size(), &_renderingData[0], isStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(ShaderAttributeID::charRect);
				glVertexAttribPointer(ShaderAttributeID::charRect, 4, GL_FLOAT, GL_FALSE, sizeof(CharRenderInfo), (GLvoid*)offsetof(CharRenderInfo, charRect));
				glVertexAttribDivisor(ShaderAttributeID::charRect, 1);

				glEnableVertexAttribArray(ShaderAttributeID::charPos);
				glVertexAttribPointer(ShaderAttributeID::charPos, 3, GL_FLOAT, GL_FALSE, sizeof(CharRenderInfo), (GLvoid*)offsetof(CharRenderInfo, charPos));
				glVertexAttribDivisor(ShaderAttributeID::charPos, 1);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
			})
		.finalize();
}

void TextRenderer::setText(const std::string& text) {
	if (_isStatic)
		return;

	if (text.size() > _maxTextLength)
		_text = text.substr(0, _maxTextLength);
	else
		_text = text;

	_rebuild();
}

void TextRenderer::_rebuild(bool upload) {
	glm::vec3 pos{0, 0, 0};
	// Calc middle
	for (unsigned int i = 0; i < _text.size(); i++) {
		const CharInfo& info = _factory._getChar(_text[i]);
		pos.x -= info.xAdvanceAmount * 1.2;
	}
	pos /= -2;

	for (unsigned int i = 0; i < _text.size(); i++) {
		const CharInfo& info = _factory._getChar(_text[i]);
		CharRenderInfo& rinfo = _renderingData[i];
		rinfo.charRect = glm::vec4{info.pos, info.size};
		rinfo.charPos = pos;
		pos.x -= info.xAdvanceAmount * 1.2;
	}

	if (upload)
		_mesh->uploadBufferArray("renderInfo", _renderingData);
}
