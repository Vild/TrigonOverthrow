#include "floorentity.hpp"

#include "../component/floortransformcomponent.hpp"
#include "../component/modelcomponent.hpp"
#include "../../engine.hpp"

#include <cstdlib>
#include <cmath>

FloorEntity::FloorEntity() : Entity(sole::rebuild("b056cfea-b2cd-4c91-b921-5b8ee6b286d6"), "Floor") {
	constexpr int gridSize = 8; // will be gridSize*gridSize

	auto transform = addComponent<FloorTransformComponent>();
	transform->gridSize = gridSize;
	transform->scale = glm::vec3(0.99, 1, 0.99);
	transform->recalculateMatrices();

#define frand() ((rand() * 1.0) / RAND_MAX)
	float* topData = new float[gridSize * gridSize];

	for (int z = 0; z < gridSize; z++)
		for (int x = 0; x < gridSize; x++) {
			topData[z * gridSize + x] = frand() * 2 - 1;
		}

	for (int z = 0; z < gridSize; z++)
		for (int x = 0; x < gridSize; x++) {
			auto p = topData[z * gridSize + x];
			const auto& forwards = z > 0 ? topData[(z - 1) * gridSize + x] : p;
			const auto& left = x < gridSize - 1 ? topData[z * gridSize + x + 1] : p;
			const auto& right = x > 0 ? topData[z * gridSize + x - 1] : p;
			const auto& backwards = z < gridSize - 1 ? topData[(z + 1) * gridSize + x] : p;

			p = ((forwards + left + right + backwards) + p * 2) / 6;
		}

	glm::vec4* neighborData = new glm::vec4[gridSize * gridSize];
	for (int z = 0; z < gridSize; z++)
		for (int x = 0; x < gridSize; x++) {
			// See floorbase.geom
			enum Side { forwards = 0, left = 1, right = 2, backwards = 3 };

			glm::vec4& point = neighborData[z * gridSize + x];

			float cur = topData[z * gridSize + x];

			float minFloor = cur - 100;

			point[forwards] = z > 0 ? topData[(z - 1) * gridSize + x] : minFloor;
			point[left] = x < gridSize - 1 ? topData[z * gridSize + x + 1] : minFloor;
			point[right] = x > 0 ? topData[z * gridSize + x - 1] : minFloor;
			point[backwards] = z < gridSize - 1 ? topData[(z + 1) * gridSize + x] : minFloor;

			point[forwards] = std::min(minFloor, point[forwards]);
			point[left] = std::min(minFloor, point[left]);
			point[right] = std::min(minFloor, point[right]);
			point[backwards] = std::min(minFloor, point[backwards]);
		}

	auto model = addComponent<ModelComponent>();
	model->meshData = Engine::getInstance().getMeshLoader()->getMesh("assets/objects/box.obj");
	model->meshData->mesh
		->addBuffer("m",
								[](GLuint id) {
									glBindBuffer(GL_ARRAY_BUFFER, id);
									glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * gridSize * gridSize, NULL, GL_DYNAMIC_DRAW);

									for (int i = 0; i < 4; i++) {
										glEnableVertexAttribArray(ShaderAttributeID::m + i);
										glVertexAttribPointer(ShaderAttributeID::m + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4) * i));
										glVertexAttribDivisor(ShaderAttributeID::m + i, 1);
									}

									glBindBuffer(GL_ARRAY_BUFFER, 0);
								})
		.addBuffer("top",
							 [&](GLuint id) {
								 glBindBuffer(GL_ARRAY_BUFFER, id);
								 glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * gridSize * gridSize, topData, GL_STATIC_DRAW);

								 glEnableVertexAttribArray(ShaderAttributeID::top);
								 glVertexAttribPointer(ShaderAttributeID::top, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), NULL);
								 glVertexAttribDivisor(ShaderAttributeID::top, 1);

								 glBindBuffer(GL_ARRAY_BUFFER, 0);
							 })
		.addBuffer("neighbor",
							 [&](GLuint id) {
								 glBindBuffer(GL_ARRAY_BUFFER, id);
								 glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * gridSize * gridSize, neighborData, GL_STATIC_DRAW);

								 glEnableVertexAttribArray(ShaderAttributeID::neighbor);
								 glVertexAttribPointer(ShaderAttributeID::neighbor, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), NULL);
								 glVertexAttribDivisor(ShaderAttributeID::neighbor, 1);

								 glBindBuffer(GL_ARRAY_BUFFER, 0);
							 })
		.finalize();
	delete[] topData;
	delete[] neighborData;
}
