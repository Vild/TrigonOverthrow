#include "game.h"

int main() {
	srand(time(0));

	Game game;

	while (game.getWindow().isOpen()) {
		sf::Event event;
		while (game.getWindow().pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				game.getWindow().close();

			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				game.getWindow().close();
		}

		// Update
		game.Update();

		// Draw
		game.Draw();
	}

	return 0;
}
