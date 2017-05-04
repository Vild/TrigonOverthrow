// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "meshloader.hpp"

#include <iostream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../engine.hpp"

static bool hasVertexColors(aiMesh* mesh, unsigned int pIndex) {
	if (pIndex >= AI_MAX_NUMBER_OF_COLOR_SETS)
		return false;
	else
		return mesh->mColors[pIndex] != NULL && mesh->mNumVertices > 0;
}

static bool hasTextureCoords(aiMesh* mesh, unsigned int pIndex) {
	if (pIndex >= AI_MAX_NUMBER_OF_TEXTURECOORDS)
		return false;
	else
		return mesh->mTextureCoords[pIndex] != NULL && mesh->mNumVertices > 0;
}

MeshLoader::MeshLoader() {}

MeshLoader::~MeshLoader() {
	_storage.clear();
}

std::shared_ptr<LoadedMesh> MeshLoader::getMesh(const std::string& file) {
	std::cout << "Want mesh: " << file << std::endl;
	try {
		std::shared_ptr<LoadedMesh> loadedMesh = std::make_shared<LoadedMesh>(file);
		return _storage[file] = loadedMesh;
	} catch (const char* msg) {
		std::cout << msg << std::endl;
	}
	return std::shared_ptr<LoadedMesh>();
}

LoadedMesh::LoadedMesh(const std::string& file) {
	Assimp::Importer importer;
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	const aiScene* scene = importer.ReadFile(
		file,
		aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenNormals | aiProcess_FlipUVs);
	if (!scene) {
		fprintf(stderr, "Could not load model %s\n", file.c_str());
		throw std::exception();
	}

	mesh = _getModel(scene);
	texture = _getTexture(scene, file);
	normalTexture = Engine::getInstance().getTextureManager()->getTexture("assets/textures/errorNormal.png");
}

std::shared_ptr<Mesh> LoadedMesh::_getModel(const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	unsigned int counterVertices = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* assimpMesh = scene->mMeshes[i];
		bool hasColors = hasVertexColors(assimpMesh, 0);
		bool hasUV = hasTextureCoords(assimpMesh, 0);
		for (unsigned int j = 0; j < assimpMesh->mNumVertices; j++) {
			Vertex vertex;

			aiVector3D p = assimpMesh->mVertices[j];
			vertex.position = glm::vec3{p.x, p.y, p.z};

			p = assimpMesh->mNormals[j];
			vertex.normal = glm::vec3{p.x, p.y, p.z};

			if (hasColors) {
				aiColor4D c = assimpMesh->mColors[0][j];
				vertex.color = glm::vec3{c.r, c.g, c.b};
			} else
				vertex.color = glm::vec3{1.f, 1.f, 1.f};

			if (hasUV) {
				aiVector3D uv = assimpMesh->mTextureCoords[0][j];
				vertex.uv = glm::vec2{uv.x, uv.y};
			} else
				vertex.uv = glm::vec2{j & 2, (j / 2) % 2};

			if (assimpMesh->mTangents) {
				p = assimpMesh->mTangents[j];
				vertex.tangent = glm::vec3{p.x, p.y, p.z};
			}

			vertices.push_back(vertex);
		}

		for (unsigned int j = 0; j < assimpMesh->mNumFaces; j++) {
			unsigned int* face = assimpMesh->mFaces[j].mIndices;
			indices.push_back(counterVertices + face[0]);
			indices.push_back(counterVertices + face[1]);
			indices.push_back(counterVertices + face[2]);
		}
		counterVertices += assimpMesh->mNumVertices;
	}

	return std::make_shared<Mesh>(vertices, indices);
}

LoadedMesh::~LoadedMesh() {}

std::shared_ptr<Texture> LoadedMesh::_getTexture(const aiScene* scene, const std::string& filename) {
	if (scene->mNumMaterials == 0)
		return Engine::getInstance().getTextureManager()->getTexture(""); // Error texture
	const aiMaterial* pMaterial = scene->mMaterials[0];
	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) == 0)
		return Engine::getInstance().getTextureManager()->getTexture(""); // Error texture

	aiString path;

	if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) != AI_SUCCESS)
		return Engine::getInstance().getTextureManager()->getTexture(""); // Error texture

	if (path.data[0] == '*') {
		unsigned int id = atoi(path.data + 1);
		printf("Embedded texture: %u(%s)\n", id, path.data);
		if (scene->mNumTextures < id)
			return Engine::getInstance().getTextureManager()->getTexture(""); // Error texture
		aiTexture* tex = scene->mTextures[id];

		printf("Texture: \n");
		printf("\tmWidth: %u\n", tex->mWidth);
		printf("\tmHeight: %u\n", tex->mHeight);
		printf("\tachFormatHint: %s\n", tex->achFormatHint);
		printf("\tpcData: %p\n", (void*)tex->pcData);
		if (tex->mHeight)
			return std::make_shared<Texture>(tex->mWidth, tex->mHeight, (void*)tex->pcData);
		else
			return std::make_shared<Texture>(tex->achFormatHint, (void*)tex->pcData, tex->mWidth);
	} else {
		std::string fullPath = filename.substr(0, filename.find_last_of("/\\") + 1) + path.data; // TODO: fix path
		printf("External texture: %s\n", fullPath.c_str());
		return Engine::getInstance().getTextureManager()->getTexture(fullPath);
	}
}
