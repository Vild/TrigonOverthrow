#include "game.h"

Game::Game() : blockSize(50.f, 50.f) {
	this->fullscreen = false;
	this->keyTime = 0;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	this->window.create(VideoMode(1920, 1080), "MapMaker", Style::Default, settings);
	this->window.setFramerateLimit(60);
	this->window.setVerticalSyncEnabled(true);

	this->mainView.setSize(this->window.getSize().x, this->window.getSize().y);

	// MAP INIT
	Vector2i mapSize(15.f, 15.f);

	this->map = new Map(mapSize, this->blockSize);

	// GUI
	this->GUI_InfoBG.setPosition(Vector2f(5.f, 5.f));
	this->GUI_InfoBG.setSize(Vector2f(250.f, 500.f));
	this->GUI_InfoBG.setFillColor(Color(100, 100, 100, 150));
	this->GUI_InfoBG.setOutlineColor(Color(255, 255, 255, 200));
	this->GUI_InfoBG.setOutlineThickness(1.f);

	// GUI TEXT
	this->font.loadFromFile("assets/fonts/trench100free.ttf");

	this->GUI_InfoText.setFont(this->font);
	this->GUI_InfoText.setCharacterSize(16.f);
	this->GUI_InfoText.setPosition(this->GUI_InfoBG.getPosition().x + 5.f, this->GUI_InfoBG.getPosition().y + 5);
	this->GUI_InfoText.setString("TEST");

	this->pausedText.setFillColor(Color::Red);
	this->pausedText.setFont(font);
	this->pausedText.setCharacterSize(24);
	this->pausedText.setString("PAUSED");
	this->pausedText.setPosition(this->window.getSize().x / 2, this->window.getSize().y / 2);

	// SELECTOR
	this->selector.setSize(Vector2f(50.f, 50.f));
	this->selector.setFillColor(Color(255, 255, 255, 0));
	this->selector.setOutlineThickness(1.f);
	this->selector.setOutlineColor(Color(255, 0, 0, 150));

	// EDITOR
	this->blockColor = 255;
	this->enemyAmmount = 0;
	this->enemyHP = 0;

	this->es_locator.setFillColor(Color::Red);
	this->es_locator.setRadius(this->blockSize.x / 2);

	this->es_text.setFont(this->font);
	this->es_text.setFillColor(Color::White);
	this->es_text.setStyle(Text::Bold);
	this->es_text.setCharacterSize(20);
	this->es_text.setString("0");

	// BUTTONS
	buttons.push_back(Butn("L_Arrow_BlockCol", Vector2f(140.f, 95.f), "-", font));
	buttons.push_back(Butn("R_Arrow_BlockCol", Vector2f(170.f, 95.f), "+", font));
	buttons.push_back(Butn("Max_BlockCol", Vector2f(10.f, 115.f), "Max", font));
	buttons.push_back(Butn("Mid_BlockCol", Vector2f(50.f, 115.f), "Mid", font));
	buttons.push_back(Butn("Min_BlockCol", Vector2f(90.f, 115.f), "Min", font));
	buttons.push_back(Butn("L_Arrow_Eamount", Vector2f(140.f, 180.f), "-", font));
	buttons.push_back(Butn("R_Arrow_Eamount", Vector2f(170.f, 180.f), "+", font));
	buttons.push_back(Butn("L_Arrow_EHP", Vector2f(140.f, 230.f), "-", font));
	buttons.push_back(Butn("R_Arrow_EHP", Vector2f(170.f, 230.f), "+", font));
}

Game::~Game() {
	delete this->map;
}

void Game::FullscreenToggle() {
	if (this->fullscreen) {
		this->fullscreen = false;
		this->window.close();
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		this->window.create(VideoMode(1920, 1080), "MapMaker", Style::Default, settings);
		this->window.setFramerateLimit(60);
		this->window.setVerticalSyncEnabled(true);

		this->mainView.setSize(this->window.getSize().x, this->window.getSize().y);
	} else {
		this->fullscreen = true;
		this->window.close();
		sf::ContextSettings settings;
		settings.antialiasingLevel = 8;
		this->window.create(VideoMode(1920, 1080), "MapMaker", Style::Fullscreen, settings);
		this->window.setFramerateLimit(60);
		this->window.setVerticalSyncEnabled(true);

		this->mainView.setSize(this->window.getSize().x, this->window.getSize().y);
	}
}

void Game::ResizeMap(Vector2f blockSize) {
	Vector2i mapSize;

	std::cout << "Map size X... ";
	std::cin >> mapSize.x;
	std::cout << "Map size Y... ";
	std::cin >> mapSize.y;
	std::cin.ignore();

	delete this->map;
	this->map = new Map(mapSize, this->blockSize);

	std::cout << "New map created...";
}

void Game::ButtonsUpdate() {
	for (size_t i = 0; i < this->buttons.size(); i++) {
		if (buttons[i].isPressed(this->mPosWindow)) {
			if (buttons[i].getName() == "L_Arrow_BlockCol" && blockColor > 0) {
				blockColor--;
			} else if (buttons[i].getName() == "R_Arrow_BlockCol" && blockColor < 255) {
				blockColor++;
			} else if (buttons[i].getName() == "Max_BlockCol") {
				blockColor = 255;
			} else if (buttons[i].getName() == "Mid_BlockCol") {
				blockColor = 128;
			} else if (buttons[i].getName() == "Min_BlockCol") {
				blockColor = 0;
			} else if (buttons[i].getName() == "L_Arrow_Eamount") {
				if (this->enemyAmmount > 0)
					this->enemyAmmount--;
			} else if (buttons[i].getName() == "R_Arrow_Eamount") {
				this->enemyAmmount++;
			} else if (buttons[i].getName() == "L_Arrow_EHP") {
				if (this->enemyHP > 0)
					this->enemyHP--;
			} else if (buttons[i].getName() == "R_Arrow_EHP") {
				this->enemyHP++;
			}
		}
	}
}

void Game::Update() {
	if (this->window.hasFocus()) {
		if (keyTime <= 8)
			this->keyTime++;

		// MOUSE UPDATE/
		mPosView = this->window.mapPixelToCoords(Mouse::getPosition(this->window));
		mPosWindow = Mouse::getPosition(this->window);
		mPosGrid = Vector2i(((int)mPosView.x / (int)this->blockSize.x), ((int)mPosView.y / (int)this->blockSize.y));
		mPosGridBounds = Vector2i(((int)mPosView.x / (int)this->blockSize.x), ((int)mPosView.y / (int)this->blockSize.y));

		// LIMITS FOR MPOSGRIDBOUNDS
		if (mPosGridBounds.x < 0)
			mPosGridBounds.x = 0;
		else if (mPosGridBounds.x >= map->getSize().x)
			mPosGridBounds.x = map->getSize().x - 1;

		if (mPosGridBounds.y < 0)
			mPosGridBounds.y = 0;
		else if (mPosGridBounds.y >= map->getSize().y)
			mPosGridBounds.y = map->getSize().y - 1;

		// IF MOUSE OUTSIDE GUI
		if (!GUI_InfoBG.getGlobalBounds().contains((Vector2f)this->mPosWindow)) {
			// SET COLOR
			if (Mouse::isButtonPressed(Mouse::Left) && keyTime > 2) {
				if (mPosGridBounds.x >= 0 && mPosGridBounds.x < map->getSize().x && mPosGridBounds.y >= 0 && mPosGridBounds.y < map->getSize().y) {
					map->getMapData()[mPosGridBounds.x][mPosGridBounds.y] = blockColor;
				}

				keyTime = 0;
			}

			// ADD ENEMY SPAWNER
			if (Mouse::isButtonPressed(Mouse::Right)) {
				if (mPosGridBounds.x >= 0 && mPosGridBounds.x < map->getSize().x && mPosGridBounds.y >= 0 && mPosGridBounds.y < map->getSize().y) {
					int tempEA = this->enemyAmmount;
					int tempEHP = this->enemyHP;

					if (this->map->getEnemySpawners().empty()) {
						if (this->enemyAmmount == 0)
							tempEA = rand() % 5 + 1;

						if (this->enemyHP == 0)
							tempEHP = rand() % 20 + 3;

						this->map->getEnemySpawners().push_back(EnemySpawner(Vector2f(mPosGridBounds.x * blockSize.x, mPosGridBounds.y * blockSize.y), tempEA, tempEHP));
					}

					else {
						bool placable = true;
						for (size_t i = 0; i < this->map->getEnemySpawners().size(); i++) {
							if (this->map->getEnemySpawners()[i].position.x == mPosGridBounds.x * blockSize.x &&
									this->map->getEnemySpawners()[i].position.y == mPosGridBounds.y * blockSize.y) {
								std::cout << "ERROR CAN'T PLACE ENEMYSPAWNER HERE! " << this->map->getEnemySpawners().size() << "\n";
								placable = false;
							}
						}
						if (placable) {
							if (this->enemyAmmount == 0)
								tempEA = rand() % 5 + 1;

							if (this->enemyHP == 0)
								tempEHP = rand() % 20 + 3;

							this->map->getEnemySpawners().push_back(EnemySpawner(Vector2f(mPosGridBounds.x * blockSize.x, mPosGridBounds.y * blockSize.y), tempEA, tempEHP));
						}
					}
				}
			}

			// REMOVE ENEMYSPAWNER
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
				bool found = false;
				for (size_t i = 0; i < this->map->getEnemySpawners().size() && !found; i++) {
					if (this->map->getEnemySpawners()[i].position.x == mPosGridBounds.x * blockSize.x &&
							this->map->getEnemySpawners()[i].position.y == mPosGridBounds.y * blockSize.y) {
						this->map->getEnemySpawners().erase(this->map->getEnemySpawners().begin() + i);
						found = true;
					}
				}
			}
		}

		// IF MOUSE IN GUI
		else if (GUI_InfoBG.getGlobalBounds().contains((Vector2f)this->mPosWindow) && keyTime > 2) {
			// BUTTONS UPDATE
			this->ButtonsUpdate();

			keyTime = 0;
		}

		// MOVE VIEW
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			this->mainView.move(-10.f, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			this->mainView.move(10.f, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			this->mainView.move(0.f, -10.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			this->mainView.move(0.f, 10.f);
		}

		// SAVE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1) && keyTime > 8) {
			std::string fileName = "";
			char answer = ' ';
			Image image;
			std::cout << "Save map... Filename (NO EXTENSION): ";
			std::cin >> fileName;
			std::cin.ignore();

			if (image.loadFromFile(fileName)) {
				std::cout << "FILE EXISTS! OVERWRITE?";
				std::cin.get(answer);

				if (answer == 'Y') {
					this->map->Save(fileName);
					std::cout << "FILE OVERWRITTEN!"
										<< "\n";
					std::cin.ignore();
				} else if (answer == 'N') {
					std::cout << "FILE NOT OVERWRITTEN..."
										<< "\n";
					std::cin.ignore();
				} else {
					std::cout << "WRONG INPUT..."
										<< "\n";
					std::cin.ignore();
				}
			} else {
				this->map->Save(fileName);
				std::cout << "FILE SAVED!"
									<< "\n";
				std::cin.ignore();
			}

			keyTime = 0;
		}

		// LOAD MAP
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F2) && keyTime > 8) {
		}

		// FULLSCREEN TOGGLE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && keyTime > 8) {
			FullscreenToggle();
			keyTime = 0;
		}

		// RESIZE
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && keyTime > 8) {
			ResizeMap(Vector2f(50.f, 50.f));
		}

		// GUI UPDATE
		std::stringstream ss;
		ss << "mPosView: " << mPosView.x << " " << mPosView.y << "\n";
		ss << "mPosWindow " << mPosWindow.x << " " << mPosWindow.y << "\n";
		ss << "mPosGrid " << mPosGrid.x << " " << mPosGrid.y << "\n";
		ss << "mPosGridBounds " << mPosGridBounds.x << " " << mPosGridBounds.y << "\n";
		ss << "\n";
		ss << "Block color " << blockColor << "\n";
		ss << "\n";
		ss << "\n";
		ss << "Enemy Spawners... "
			 << "\n";
		ss << "Enemy count (0 = rand) " << this->enemyAmmount << "\n";
		ss << "\n";
		ss << "\n";
		ss << "Enemy HP (0 = rand) " << this->enemyHP << "\n";
		ss << "\n";
		ss << "\n";
		ss << "Hotkeys... "
			 << "\n";
		ss << "Move camera -  WASD keys "
			 << "\n";
		ss << "Color block - Left Mouse "
			 << "\n";
		ss << "Place spawner - Right Mouse "
			 << "\n";
		ss << "Remove spawner - Delete "
			 << "\n";
		ss << "Fullscreen - F11 "
			 << "\n";
		ss << "Save - F1 "
			 << "\n";
		ss << "New map - R "
			 << "\n";
		ss << "Exit without saving - ESC "
			 << "\n";
		GUI_InfoText.setString(ss.str());

		// SELECTOR FOR BLOCKS
		if (mPosGridBounds.x >= 0 && mPosGridBounds.y >= 0 && mPosGridBounds.x < this->map->getSize().x && mPosGridBounds.y < this->map->getSize().y)
			selector.setPosition((Vector2f(mPosGridBounds.x * this->blockSize.x, mPosGridBounds.y * this->blockSize.y)));
	}
}

void Game::Draw() {
	this->window.clear();

	// EVERYTHING
	this->window.setView(this->mainView);

	this->map->Draw(this->window);

	for (size_t i = 0; i < this->map->getEnemySpawners().size(); i++) {
		es_locator.setPosition(this->map->getEnemySpawners()[i].position);

		es_locator.setPointCount(this->map->getEnemySpawners()[i].eHP);

		es_text.setPosition(es_locator.getPosition().x + es_locator.getRadius() / 3, es_locator.getPosition().y + es_locator.getRadius() / 3);
		es_text.setString(std::to_string(this->map->getEnemySpawners()[i].eCount) + " " + std::to_string(this->map->getEnemySpawners()[i].eHP));

		window.draw(es_locator);
		window.draw(es_text);
	}

	this->window.draw(this->selector);

	// GUI
	this->window.setView(this->window.getDefaultView());
	this->window.draw(this->GUI_InfoBG);
	this->window.draw(this->GUI_InfoText);

	for (size_t i = 0; i < this->buttons.size(); i++) {
		this->buttons[i].Draw(this->window);
	}

	this->window.display();

	this->window.setView(this->mainView);
}
