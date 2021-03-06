#pragma once

#include <string>
#include <assimp/Importer.hpp>

#include "../gl/mesh.hpp"
#include "../gl/texture.hpp"

struct LoadedMesh {
	LoadedMesh(const std::string& file);
	LoadedMesh(const LoadedMesh& other);
	virtual ~LoadedMesh();

	std::unique_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<Texture> normalTexture;

private:
	std::unique_ptr<Mesh> _getModel(const aiScene* scene);
	std::shared_ptr<Texture> _getTexture(const aiScene* scene, const std::string& filename);
};

class MeshLoader {
public:
	MeshLoader();
	virtual ~MeshLoader();

	std::shared_ptr<LoadedMesh> getMesh(const std::string& file);

private:
	std::map<std::string, LoadedMesh*> _storage;
};
