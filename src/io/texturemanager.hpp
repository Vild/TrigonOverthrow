#pragma once

#include <map>
#include <memory>

#include "../gl/texture.hpp"

class TextureManager {
public:
	TextureManager();
	virtual ~TextureManager();

	std::shared_ptr<Texture> getTexture(const std::string& file);
	std::shared_ptr<Texture> getErrorTexture();

private:
	std::map<std::string, std::shared_ptr<Texture>> _storage;
	std::shared_ptr<Texture> _errorTexture;

	std::shared_ptr<Texture> _loadErrorTexture();
};
