#pragma once

#include <string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include "../gl/mesh.hpp"

#include "../gl/texture.hpp"

struct CharRenderInfo {
	// charRect.xy = [startX, startY] in texture
	// charRect.zw = [width, height] of char
	// charPos     = [xPos, yPos, zPos]
	glm::vec4 charRect;
	glm::vec3 charPos;
};

struct CharInfo {
	glm::vec2 pos;
	glm::vec2 size;

	// glm::vec2 offset;
	float xAdvanceAmount;
};

class TextRenderer;

class TextFactory {
public:
	friend class TextRenderer;
	TextFactory(const std::string& fontFile);
	virtual ~TextFactory();

	std::shared_ptr<TextRenderer> makeRenderer(const std::string& text, bool isStatic = false, unsigned int maxTextLength = 64);

	inline std::shared_ptr<Texture> getFontMap() { return _fontMap; };

private:
	const int _ptSize = 30;

	std::shared_ptr<Texture> _fontMap;
	CharInfo _charInfos[256];

	std::vector<std::shared_ptr<TextRenderer>> _renderers;

	inline const CharInfo& _getChar(char ch) { return _charInfos[(int)ch]; }
};

class TextRenderer {
public:
	/// maxTextLength will only be used when isStatic = false
	TextRenderer(TextFactory& factory, const std::string& text, bool isStatic, unsigned int maxTextLength);
	virtual ~TextRenderer();

	void setText(const std::string& text);
	inline const std::string& getText() { return _text; }
	inline bool isStatic() { return _isStatic; }
	inline unsigned int getMaxTextLength() { return _maxTextLength; }
	inline std::unique_ptr<Mesh>& getMesh() { return _mesh; }

private:
	TextFactory& _factory;
	std::string _text;
	bool _isStatic;
	unsigned int _maxTextLength;

	std::unique_ptr<Mesh> _mesh;
	std::vector<CharRenderInfo> _renderingData;

	void _rebuild(bool upload = true);
};
