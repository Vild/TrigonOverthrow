#pragma once

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <sstream>

using namespace sf;

class Butn {
private:
	int ID;
	std::string name;
	Texture* texture;
	RectangleShape sprite;
	Text btnText;

public:
	Butn(std::string name, Vector2f pos, std::string btnText, Font& font);
	virtual ~Butn();

	void setPosition(Vector2f pos);
	inline const std::string& getName() const { return this->name; }
	void isHover(const Vector2i& mPosWindow);
	bool isPressed(const Vector2i& mPosWindow);
	void Update(const Vector2i& mPosWindow);
	void Draw(RenderTarget& target);

	// STATIC
	static int B_ID;
};
