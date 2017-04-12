#pragma once

#include <string>
#include <SDL2/SDL_ttf.h>
#include <memory>

#include "../gl/texture.hpp"

enum class FontStyle {
	normal = TTF_STYLE_NORMAL,
	bold = TTF_STYLE_BOLD,
	italic = TTF_STYLE_ITALIC,
	underline = TTF_STYLE_UNDERLINE,
	strikethrough = TTF_STYLE_STRIKETHROUGH
};

FontStyle operator|(FontStyle a, FontStyle b) {
	return static_cast<FontStyle>((int)a + (int) b);
}

class FontFactory {
public:
	FontFactory();
	~FontFactory();

	std::shared_ptr<Texture> render(const std::string& str, FontStyle fontStyle = FontStyle::normal, int outline = 0);

private:
	const std::string _fontFile = "assets/fonts/SF Theramin Gothic Bold.ttf";
	const int _ptSize = 30;

	TTF_Font * _font;
};
