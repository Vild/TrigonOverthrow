#include "textfactory.hpp"

FontFactory::FontFactory() {
	_font = TTF_OpenFont(_fontFile.c_str(), _ptSize);
	if (!_font)
		throw "Failed to load font!";
}

FontFactory::~FontFactory() {
	TTF_CloseFont(_font);
}


std::shared_ptr<Texture> FontFactory::render(const std::string& str, FontStyle fontStyle, int outline) {
	TTF_SetFontStyle(_font, (int)fontStyle);
	TTF_SetFontOutline(_font, outline);

	TTF_RenderUTF8_Solid()
}
