// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "texturemanager.hpp"

#include <iostream>
#include <exception>

TextureManager::TextureManager() : _errorTexture(_loadErrorTexture()) {}

TextureManager::~TextureManager() {
	_storage.clear();
}

std::shared_ptr<Texture> TextureManager::getTexture(const std::string& file) {
	std::cout << "Want file: " << file << std::endl;
	try {
		std::shared_ptr<Texture> tex = std::make_shared<Texture>(file);
		return _storage[file] = tex;
	} catch (const char* msg) {
		std::cout << msg << std::endl;
	}
	return _storage[file] = _errorTexture;
}

std::shared_ptr<Texture> TextureManager::getErrorTexture() {
	return _errorTexture;
}

std::shared_ptr<Texture> TextureManager::_loadErrorTexture() {
	return std::make_shared<Texture>("assets/textures/error.png");
}
