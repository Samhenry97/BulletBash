#include "logger.h"
#include "const.h"
#include "game.h"
#include "images.h"
#include "sounds.h"
#include "util.h"
#include "xbox.h"

void pollEvents(sf::RenderWindow &window) {
	sf::Event e;
	int id, button, axis;
	float pos;

	while (window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			running = false;
			break;
		case sf::Event::JoystickButtonPressed:
			id = e.joystickButton.joystickId, button = e.joystickButton.button;
			Xbox::setButton(id, button, true);
			game->sendButtonPressed(id, button);
			break;
		case sf::Event::JoystickButtonReleased:
			id = e.joystickButton.joystickId, button = e.joystickButton.button;
			Xbox::setButton(id, button, false);
			game->sendButtonReleased(id, button);
			break;
		case sf::Event::JoystickMoved:
			id = e.joystickMove.joystickId, axis = e.joystickMove.axis, pos = e.joystickMove.position;
			Xbox::setAxis(id, axis, pos);
			game->sendAxisMoved(id, axis, pos);
			break;
		case sf::Event::JoystickConnected:
			id = e.joystickConnect.joystickId;
			Xbox::setConnected(id, true);
			break;
		case sf::Event::JoystickDisconnected:
			id = e.joystickConnect.joystickId;
			Xbox::setConnected(id, false);
			break;
		case sf::Event::LostFocus:
			Logger::info("Lost focus!");
			break;
		case sf::Event::GainedFocus:
			Logger::info("Gained focus!");
			break;
		case sf::Event::Resized:
			Logger::info("Resized!");
			break;
		case sf::Event::KeyPressed:
			Logger::info("Key pressed!");
			break;
		}
	}
}

bool drawMinimap() {
	for (int i = 0; i < 4; i++) {
		if (Xbox::isConnected(i)) {
			float angle = Xbox::getAxis(i, XBOX_TRIGGERS);
			if (angle > 0.1f) return true;
		}
	}
	return false;
}

int main() {
	sf::RenderWindow display(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), NAME, sf::Style::Fullscreen);
	display.setVerticalSyncEnabled(true);
	display.setFramerateLimit(60);
	display.setKeyRepeatEnabled(false);
	window = &display;
	view = new sf::View(vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	minimap = new sf::View;
	minimap->setViewport(sf::FloatRect(MINIMAP_X, MINIMAP_Y, MINIMAP_WIDTH, MINIMAP_HEIGHT));
	minimap->setSize(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2);

	sf::RectangleShape minimapBackground;
	minimapBackground.setSize(vec2(WINDOW_WIDTH * MINIMAP_WIDTH, WINDOW_HEIGHT * MINIMAP_HEIGHT));
	minimapBackground.setPosition(WINDOW_WIDTH * MINIMAP_X, WINDOW_HEIGHT * MINIMAP_Y);
	minimapBackground.setTexture(Images::get("gui.png"));
	minimapBackground.setFillColor(sf::Color(255, 255, 255, 200));

	sf::RectangleShape background;
	background.setSize(vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
	background.setTexture(Images::get("background.png"));

	Sounds::init();
	Xbox::init();
	srand(time(0));
	game = new Game();

	sf::Clock timer;
	running = true;
	while (running) {
		pollEvents(display);
		game->update();
		frameTime = timer.restart().asSeconds();

		display.clear(sf::Color::Black);
		window->setView(window->getDefaultView());
		window->draw(background);
		window->setView(*view);
		game->render();
		window->setView(window->getDefaultView());
		game->renderStatic();
		if (drawMinimap()) {
			window->draw(minimapBackground);
			window->setView(*minimap);
			game->renderMinimap();
		}

		display.display();
	}

	delete game;

	return 0;
}
