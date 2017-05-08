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
	if (file.empty())
		return _errorTexture;
	std::shared_ptr<Texture> texture = _storage[file];
	if (!texture) {
		try {
			std::cout << "Loading texture: " << file << std::endl;
			texture = _storage[file] = std::make_shared<Texture>(file);
		} catch (const std::exception& e) {
			std::cerr << "FAILED TO LOAD TEXTURE: " << e.what() << std::endl;
			return _errorTexture;
		} catch (const char* msg) {
			std::cerr << "FAILED TO LOAD TEXTURE: " << msg << std::endl;
			return _errorTexture;
		}
	}

	return texture;
}

std::shared_ptr<Texture> TextureManager::getErrorTexture() {
	return _errorTexture;
}

std::shared_ptr<Texture> TextureManager::_loadErrorTexture() {
	return std::make_shared<Texture>("assets/textures/error.png");
}
