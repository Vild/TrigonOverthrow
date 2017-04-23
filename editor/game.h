#pragma once

#include "map.h"
#include "butn.h"

class Game {
private:
	RenderWindow window;

	View mainView;

	Vector2f mPosView;
	Vector2i mPosWindow;
	Vector2i mPosGrid;
	Vector2i mPosGridBounds;

	Vector2f blockSize;

	bool fullscreen;
	int keyTime;

	Map* map;

	RectangleShape GUI_InfoBG;
	Font font;
	Text GUI_InfoText;
	RectangleShape selector;
	Text pausedText;

	// EDITOR
	int blockColor;
	int enemyHP;
	int enemyAmmount;

	CircleShape es_locator;
	Text es_text;

	// BUTTONS
	std::vector<Butn> buttons;

public:
	Game();
	virtual ~Game();

	void FullscreenToggle();
	void ResizeMap(Vector2f blockSize);
	void ButtonsUpdate();
	void Update();
	void Draw();

	inline RenderWindow& getWindow() { return this->window; }
};
