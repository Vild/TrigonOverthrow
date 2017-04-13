#pragma once

#include <string>
#include <SDL2/SDL_ttf.h>
#include <memory>

#include <glm/glm.hpp>
#include "../gl/mesh.hpp"

#include "../gl/texture.hpp"

enum class FontStyle {
	normal = TTF_STYLE_NORMAL,
	bold = TTF_STYLE_BOLD,
	italic = TTF_STYLE_ITALIC,
	underline = TTF_STYLE_UNDERLINE,
	strikethrough = TTF_STYLE_STRIKETHROUGH
};

FontStyle operator|(FontStyle a, FontStyle b) {
	return static_cast<FontStyle>((int)a + (int)b);
}

struct TextBuffer {
	glm::vec4 charRect;
	glm::vec2 charPos;
};


class TextRenderer {
public:

	void render();

private:
	FontFactory& _factory;
	std::vector<
	bool _static;


	void _rebuild();

};

// assets/fonts/SF Theramin Gothic Bold.ttf
class FontFactory {
public:
	FontFactory(const std::string& fontFile);
	~FontFactory();

	TextRenderer makeStaticRenderer(const std::string& str, FontStyle fontStyle = FontStyle::normal, int outline = 0);
	TextRenderer makeDynamicRenderer(const std::string& str, FontStyle fontStyle = FontStyle::normal, int outline = 0);

private:
	struct CharInfo {
		glm::vec2 pos;
		glm::vec2 size;

		glm::vec2 offset;
		int xAdvanceAmount;
	};

	const int _ptSize = 30;

	TTF_Font* _font;
	CharInfo _charInfos[256];
	Rendere;

	inline const CharInfo& _getChar(char ch) { return _charInfos[ch]; }
};
