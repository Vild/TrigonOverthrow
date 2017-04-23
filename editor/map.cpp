#include "map.h"

Map::Map(Vector2i mapSize, Vector2f blockSize) {
	this->size = mapSize;

	this->renderTex.create(this->size.x * blockSize.x, this->size.y * blockSize.y);

	for (int row = 0; row < mapSize.x; row++) {
		this->mapData.push_back(std::vector<int>());

		for (int col = 0; col < mapSize.y; col++) {
			this->mapData[row].push_back(255);
		}
	}

	block.setSize(blockSize);
}

Map::~Map() {}

void Map::Save(std::string fileName) {
	// SAVE BLOCK HEIGHTS
	this->renderTex.clear();

	RectangleShape block;

	for (unsigned int row = 0; row < this->size.x; row++) {
		for (unsigned int col = 0; col < this->size.y; col++) {
			block.setFillColor(Color(this->mapData[row][col], this->mapData[row][col], this->mapData[row][col], 255));
			block.setPosition(Vector2f(row * block.getSize().x, col * block.getSize().y));
			renderTex.draw(block);
		}
	}

	this->renderTex.display();

	sf::Image image;
	image.create(size.x, size.y, sf::Color::Black);

	for (unsigned int row = 0; row < this->size.x; row++) {
		for (unsigned int col = 0; col < this->size.y; col++) {
			image.setPixel(row, col, Color(mapData[row][col], mapData[row][col], mapData[row][col], 255));
		}
	}

	image.saveToFile("maps/" + fileName + ".png");

	// SAVE OBJECT DATA
	std::ofstream out;

	out.open("maps/" + fileName + ".toobj");

	if (out.is_open()) {
		for (size_t i = 0; i < this->enemySpawners.size(); i++) {
			out << enemySpawners[i].position.x << " " << enemySpawners[i].position.y << " " << enemySpawners[i].eCount << " " << enemySpawners[i].eHP << "\n";
		}
	} else
		std::cout << "ERROR OPENING FILE " << (fileName) << "\n";

	out.close();
}

void Map::Draw(RenderTarget& target) {
	this->renderTex.clear(Color::White);

	for (unsigned int row = 0; row < this->size.x; row++) {
		for (unsigned int col = 0; col < this->size.y; col++) {
			block.setFillColor(Color(this->mapData[row][col], this->mapData[row][col], this->mapData[row][col], 255));
			block.setPosition(Vector2f(row * block.getSize().x, col * block.getSize().y));
			renderTex.draw(block);

			/*std::cout << block.getPosition().x << " " <<  block.getPosition().y << "\n";*/
		}
	}

	this->renderTex.display();

	this->renderSprite.setTexture(this->renderTex.getTexture());

	target.draw(this->renderSprite);
}
