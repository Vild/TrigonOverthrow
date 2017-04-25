#pragma once

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <sstream>
#include <fstream>

using namespace sf;

class EnemySpawner {
public:
	EnemySpawner(Vector2f position, int eCount, int eHP) {
		this->position = position;
		this->eCount = eCount;
		this->eHP = eHP;
	}

	virtual ~EnemySpawner() {}

	Vector2f position;
	int eCount;
	int eHP;
};

class Map {
private:
	Vector2i size;

	RenderTexture renderTex;
	Sprite renderSprite;

	std::vector<std::vector<int> > mapData;
	std::vector<EnemySpawner> enemySpawners;

	RectangleShape block;

public:
	Map(Vector2i mapSize = Vector2i(5, 5), Vector2f blockSize = Vector2f(50.f, 50.f));
	virtual ~Map();

	void Save(std::string fileName);
	void Draw(RenderTarget& target);
	inline RenderTexture& getRenderTex() { return this->renderTex; }
	inline const Vector2i& getSize() const { return this->size; }
	inline std::vector<std::vector<int> >& getMapData() { return this->mapData; }
	inline std::vector<EnemySpawner>& getEnemySpawners() { return this->enemySpawners; }

	inline void debugPrint() {
		for (size_t row = 0; row < mapData.size(); row++) {
			std::cout << std::endl;
			for (size_t col = 0; col < mapData[row].size(); col++) {
				std::cout << mapData[row][col] << " ";
			}
		}
	}
};
