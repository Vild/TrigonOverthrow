#include "butn.h"

int Butn::B_ID = 0;

Butn::Butn(std::string name, Vector2f pos, std::string btnText, Font& font) {
	this->ID = B_ID;
	this->name = name;

	this->sprite.setSize(Vector2f(15.f, 15.f));
	this->sprite.setFillColor(Color(100, 100, 100, 255));
	this->sprite.setPosition(pos);

	this->btnText.setFont(font);
	this->btnText.setString(btnText);
	this->btnText.setCharacterSize(12);
	this->btnText.setFillColor(Color::White);
	this->btnText.setPosition(this->sprite.getPosition());

	B_ID++;
}

Butn::~Butn() {}

void Butn::setPosition(Vector2f pos) {
	this->sprite.setPosition(pos);
}

void Butn::isHover(const Vector2i& mPosWindow) {
	if (this->sprite.getGlobalBounds().contains((Vector2f)mPosWindow)) {
		this->sprite.setFillColor(Color(200, 200, 200, 255));
	} else {
		this->sprite.setFillColor(Color(150, 150, 150, 255));
	}
}

bool Butn::isPressed(const Vector2i& mPosWindow) {
	if (this->sprite.getGlobalBounds().contains((Vector2f)mPosWindow) && Mouse::isButtonPressed(Mouse::Left)) {
		this->sprite.setFillColor(Color(255, 255, 255, 255));
		return true;
	} else if (this->sprite.getGlobalBounds().contains((Vector2f)mPosWindow)) {
		this->sprite.setFillColor(Color(150, 150, 150, 255));
		return false;
	} else {
		this->sprite.setFillColor(Color(100, 100, 100, 255));
		return false;
	}
}

void Butn::Update(const Vector2i& mPosWindow) {
	isHover(mPosWindow);
}

void Butn::Draw(RenderTarget& target) {
	target.draw(this->sprite);
	target.draw(this->btnText);
}
